#include "GameInterface.h"

#include "Loopie/Core/Log.h"
#include "Loopie/Core/Application.h"
#include "Loopie/Render/Renderer.h"
#include "Loopie/Components/Transform.h"
#include "Loopie/Components/MeshRenderer.h"
#include "Loopie/Components/Button.h"
#include "Loopie/Components/Text.h"
#include "Loopie/Scene/Entity.h"

#include <imgui.h>


namespace Loopie {

	GameInterface::GameInterface() {
		m_buffer = std::make_shared<FrameBuffer>(1, 1);
	}

	void GameInterface::Render() {
		
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoNav;
		if (ImGui::Begin("Game", nullptr, flags)) {
			m_visible = true;
			ImVec2 size = ImGui::GetContentRegionAvail();
			m_windowSize = { (int)size.x, (int)size.y };
			Application::GetInstance().GetInputEvent().SetGameSize(vec2(m_windowSize.x, m_windowSize.y));
			m_entityUnderMouse = MousePick();
			HandleButtonLogic();
			HandleTextLogic();
			ImGui::Image((ImTextureID)m_buffer->GetTextureId(), size, ImVec2(0, 1), ImVec2(1, 0));
		}
		else
			m_visible = false;

		ImGui::End();
	}

	Camera* GameInterface::GetCamera()
	{
		return Camera::GetMainCamera();
	}

	Loopie::Ray GameInterface::MouseRay()
	{
		vec2 mousePos = vec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);

		// calcular posición relativa dentro del panel de juego
		ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
		vec2 mouseRelative = mousePos - vec2(cursorScreenPos.x, cursorScreenPos.y);

		float ndcX = (2.0f * mouseRelative.x) / m_windowSize.x - 1.0f;
		float ndcY = 1.0f - (2.0f * mouseRelative.y) / m_windowSize.y; // flip Y

		vec4 nearPointNDC(ndcX, ndcY, -1.0f, 1.0f);
		vec4 farPointNDC(ndcX, ndcY, 1.0f, 1.0f);

		matrix4 invViewProj = glm::inverse(GetCamera()->GetViewProjectionMatrix());

		vec4 nearWorld = invViewProj * nearPointNDC;
		nearWorld /= nearWorld.w;

		// rayo ortográfico → dirección de la cámara
		vec3 origin = vec3(nearWorld);
		vec3 direction = -GetCamera()->GetTransform()->Forward();

		return Ray{ origin, direction, std::numeric_limits<float>::max() };
	}

	void GameInterface::HandleButtonLogic()
	{
		bool mousePressed = ImGui::IsMouseDown(ImGuiMouseButton_Left);
		bool mouseReleased = ImGui::IsMouseReleased(ImGuiMouseButton_Left);

		if (m_entityUnderMouse != m_lastHoveredEntity) {
			if (m_lastHoveredEntity) {
				if (auto btn = m_lastHoveredEntity->GetComponent<Button>())
					btn->SetState(ButtonState::Normal);
			}
			if (m_entityUnderMouse) {
				if (auto btn = m_entityUnderMouse->GetComponent<Button>())
					if (btn->interactive)
						btn->SetState(ButtonState::Hovered);
					else
						btn->SetState(ButtonState::Disabled);
			}
			m_lastHoveredEntity = m_entityUnderMouse;
		}

		// --- LÓGICA DE CLICK / PRESS ---
		if (m_entityUnderMouse) {
			if (auto btn = m_entityUnderMouse->GetComponent<Button>()) {
				if (mousePressed) {
					btn->SetState(ButtonState::Pressed);
					m_lastPressedEntity = m_entityUnderMouse;
				}
				else if (mouseReleased && m_lastPressedEntity == m_entityUnderMouse) {
					btn->SetState(ButtonState::Hovered);
					m_lastPressedEntity = nullptr;
				}
			}
		}

		if (mouseReleased) {
			m_lastPressedEntity = nullptr;
		}
	}

	void GameInterface::HandleTextLogic()
	{
		bool mouseReleased = ImGui::IsMouseReleased(ImGuiMouseButton_Left);

		if (mouseReleased)
		{
			m_activeTextEntity = nullptr;

			if (m_entityUnderMouse)
			{
				if (auto text = m_entityUnderMouse->GetComponent<Text>())
				{
					if (text->interactable)
						m_activeTextEntity = m_entityUnderMouse;
				}
			}
		}

		if (!m_activeTextEntity)
			return;

		Text* text = m_activeTextEntity->GetComponent<Text>();
		if (!text)
			return;

		// --- Escritura de texto ---
		ImGuiIO& io = ImGui::GetIO();

		for (int i = 0; i < io.InputQueueCharacters.Size; i++)
		{
			unsigned int c = io.InputQueueCharacters[i];

			if (c == '\b') // backspace
			{
				std::string str = text->GetText();
				if (!str.empty())
					str.pop_back();
				text->SetText(str);
			}
			else if (c == '\n' || c == '\r')
			{
				m_activeTextEntity = nullptr; // terminar edición
			}
			else if (c >= 32) // caracteres imprimibles
			{
				std::string str = text->GetText();
				str.push_back((char)c);
				text->SetText(str);
			}
		}

		io.InputQueueCharacters.clear();
	}

	std::shared_ptr<Entity> GameInterface::MousePick()
	{
		Ray mouseRay = MouseRay();
		float minDistance = std::numeric_limits<float>::max();
		std::shared_ptr<Entity> selectedEntity;

		std::unordered_set<std::shared_ptr<Entity>> possibleEntities;
		Application::GetInstance().GetScene().GetOctree().CollectIntersectingObjectsWithRay(
			mouseRay.StartPoint(), mouseRay.Direction(), possibleEntities
		);

		for (auto& entity : possibleEntities)
		{
			if (!entity->GetIsActive())
				continue;

			MeshRenderer* renderer = entity->GetComponent<MeshRenderer>();
			if (!renderer || !renderer->GetIsActive() || !renderer->GetMesh())
				continue;

			const AABB& aabb = renderer->GetWorldAABB();
			if (!aabb.IntersectsRay(mouseRay.StartPoint(), mouseRay.EndPoint()))
			{
				continue;
			}

			const MeshData& meshData = renderer->GetMesh()->GetData();
			Triangle triangle;
			std::vector<vec3> triVertexData(3);
			vec3 meshHitPoint;

			unsigned int triangleCount = meshData.Indices.size() / 3;
			for (unsigned int i = 0; i < triangleCount; i++)
			{
				if (!renderer->GetTriangle(i, triangle)) continue;
				triVertexData[0] = triangle.v0;
				triVertexData[1] = triangle.v1;
				triVertexData[2] = triangle.v2;
			
				if (!mouseRay.Intersects(triVertexData, true, meshHitPoint))
					continue;

				float distance = glm::distance(mouseRay.StartPoint(), meshHitPoint);
				if (distance < minDistance)
				{
					minDistance = distance;
					selectedEntity = entity;
				}
			}
		}

		return selectedEntity;

	}

	void GameInterface::StartScene()
	{
		m_buffer->Bind();

		if (!Camera::GetMainCamera()) {
			m_buffer->Clear();
			return;
		}

		ivec2 textureSize = ivec2(m_buffer->GetWidth(), m_buffer->GetHeight());
		vec4 viewportSize = Camera::GetMainCamera()->GetViewport();
		Renderer::SetViewport(0, 0, (unsigned int)m_windowSize.x, (unsigned int)m_windowSize.y);

		if (m_windowSize.x != textureSize.x || m_windowSize.y != textureSize.y)
			m_buffer->Resize(m_windowSize.x, m_windowSize.y);
		if(m_windowSize.x != viewportSize.z || m_windowSize.y != viewportSize.w)
			Camera::GetMainCamera()->SetViewport(0, 0, m_windowSize.x, m_windowSize.y);

		m_buffer->Clear();
	}

	void GameInterface::EndScene()
	{
		m_buffer->Unbind();
	}
}