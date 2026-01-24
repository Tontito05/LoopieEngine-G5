#include "HierarchyInterface.h"
#include "Loopie/Core/Log.h"
#include "Loopie/Components/MeshRenderer.h"
#include "Loopie/Resources/ResourceManager.h"
#include "Loopie/Importers/MeshImporter.h"
#include "Loopie/Components/RectTransform.h"
#include "Loopie/Components/Canvas.h"
#include "Loopie/Components/CanvasScaler.h"
#include "Loopie/Components/Transform.h"
#include "Loopie/Components/Image.h"
#include "Loopie/Components/Button.h"

#include "Loopie/Helpers/LoopieHelpers.h"
#include "Editor/Interfaces/Workspace/SceneInterface.h"
#include <imgui.h>

namespace Loopie {
	std::weak_ptr<Entity> HierarchyInterface::s_SelectedEntity;
	Event<OnEntityOrFileNotification> HierarchyInterface::s_OnEntitySelected;

	HierarchyInterface::HierarchyInterface() {
		
	}

	void HierarchyInterface::Update(const InputEventManager& inputEvent)
	{
		if(m_focused)
			HotKeysSelectedEntiy(inputEvent);	
	}

	void HierarchyInterface::Render() {

		if (ImGui::Begin("Hierarchy")) {

			m_focused = ImGui::IsWindowHovered();
			
			if (!m_scene) {
				ImGui::End();
				return;
			}

			ImVec2 cursorPos = ImGui::GetCursorPos();

			if (ImGui::IsWindowHovered() && (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)))
				SelectEntity(nullptr);

			if (ImGui::BeginPopupContextWindow("HierarchyBackgroundContext", ImGuiPopupFlags_MouseButtonRight)) {
				DrawContextMenu(nullptr);
				ImGui::EndPopup();
			}

			for (const auto& entity : m_scene->GetRootEntity()->GetChildren())
			{
				DrawEntitySlot(entity);
				
			}
		

			ImVec2 size = ImGui::GetContentRegionAvail();
			//ImGui::SetCursorPos(cursorPos);  /// And move up the avail
			ImGui::SetNextItemAllowOverlap();
			ImGui::InvisibleButton("##DropTarget", size, ImGuiButtonFlags_None);
			Drop(m_scene->GetRootEntity());
		}
		ImGui::End();
	}

	void HierarchyInterface::SetScene(Scene* scene)
	{
		m_scene = scene;
	}

	void HierarchyInterface::SelectEntity(std::shared_ptr<Entity> entity)
	{
		s_SelectedEntity = entity;
		s_OnEntitySelected.Notify(OnEntityOrFileNotification::OnEntitySelect);
	}

	void HierarchyInterface::DrawEntitySlot(const std::shared_ptr<Entity>& entity)
	{
		if (!entity) {
			return;
		}
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		const auto& children = entity->GetChildren();
		bool hasChildren = !children.empty();

		if (!hasChildren)
			flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		if (s_SelectedEntity.lock() == entity)
			flags |= ImGuiTreeNodeFlags_Selected;

		bool opened = ImGui::TreeNodeEx((void*)entity.get(), flags, entity->GetName().c_str());

		Drag(entity);
		Drop(entity);

		if (ImGui::IsItemClicked())
		{
			////Expand To Select Multiple
			SelectEntity(entity);
		}

		if (ImGui::BeginPopupContextItem())
		{
			DrawContextMenu(entity);
			ImGui::EndPopup();
		}

		if (opened && hasChildren)
		{
			for (const auto& child : children)
			{
				DrawEntitySlot(child);
			}
			ImGui::TreePop();
		}
	}

	void HierarchyInterface::DrawContextMenu(const std::shared_ptr<Entity>& entity)
	{

		//// EXPAND MAYBE WITH A CUSTOM CREATOR -> MenuItem class (contains an Execute function, label, active Condition)???

		if (ImGui::MenuItem("Create Empty"))
		{
			std::shared_ptr<Entity> newEntity = m_scene->CreateEntity("Entity", entity);
			SelectEntity(newEntity);
		}	

		/*if (ImGui::MenuItem("Copy"))
		{

		}

		if (ImGui::MenuItem("Cut"))
		{

		}

		if (ImGui::MenuItem("Paste"))
		{

		}*/

		if (ImGui::MenuItem("Delete",nullptr, false, entity != nullptr))
		{
			if (s_SelectedEntity.lock() == entity)
				SelectEntity(nullptr);
			m_scene->RemoveEntity(entity->GetUUID());
		}

		ImGui::Separator();

		if (ImGui::BeginMenu("3D Object"))
		{
			if (ImGui::MenuItem("Cube"))
				SelectEntity(CreatePrimitiveModel("assets/models/primitives/cube.fbx", "Cube", entity));

			if (ImGui::MenuItem("Sphere"))
				SelectEntity(CreatePrimitiveModel("assets/models/primitives/sphere.fbx", "Sphere", entity));

			if (ImGui::MenuItem("Cylinder"))
				SelectEntity(CreatePrimitiveModel("assets/models/primitives/cylinder.fbx", "Cylinder", entity));

			if (ImGui::MenuItem("Plane"))
				SelectEntity(CreatePrimitiveModel("assets/models/primitives/plane.fbx", "Plane", entity));

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("UI"))
		{
			if (ImGui::MenuItem("Canvas"))
				SelectEntity(CreateCanvas(entity));
			if (ImGui::MenuItem("Image")) {
				if(s_SelectedEntity.lock() && s_SelectedEntity.lock()->HasComponent<Canvas>())
					SelectEntity(CreateImage(s_SelectedEntity.lock()));
			}
			if (ImGui::MenuItem("Button")) {
				if (s_SelectedEntity.lock() && s_SelectedEntity.lock()->HasComponent<Canvas>())
					SelectEntity(CreateButton(s_SelectedEntity.lock()));
			}
			ImGui::EndMenu();
		}
	}

	void HierarchyInterface::HotKeysSelectedEntiy(const InputEventManager& inputEvent)
	{
		auto selectedEntity = s_SelectedEntity.lock();
		if (!selectedEntity) {
			return;
		}

		if (inputEvent.GetKeyStatus(SDL_SCANCODE_DELETE) == KeyState::DOWN) {
			m_scene->RemoveEntity(selectedEntity->GetUUID());
			SelectEntity(nullptr);
		}

		if (inputEvent.GetKeyWithModifier(SDL_SCANCODE_C, KeyModifier::CTRL)) {
			/// Copy
		}

		if (inputEvent.GetKeyWithModifier(SDL_SCANCODE_V, KeyModifier::CTRL)) {
			/// Paste
		}

		if (inputEvent.GetKeyWithModifier(SDL_SCANCODE_X, KeyModifier::CTRL)) {
			/// Cut
		}

	}
	void HierarchyInterface::Drag(const std::shared_ptr<Entity>& entity)
	{
		if (ImGui::BeginDragDropSource())
		{
			Entity* rawPtr = entity.get();
			ImGui::SetDragDropPayload("HIERARCHY_ENTITY_PTR", &rawPtr, sizeof(Entity*));
			ImGui::EndDragDropSource();
		}
	}

	void HierarchyInterface::Drop(const std::shared_ptr<Entity>& entity)
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY_PTR"))
			{
				Entity* draggedRaw = *(Entity**)payload->Data;

				if(draggedRaw)
					draggedRaw->SetParent(entity);
			}

			ImGui::EndDragDropTarget();
		}
	}


	std::shared_ptr<Entity> HierarchyInterface::CreatePrimitiveModel(const std::string& modelPath, const std::string& name, const std::shared_ptr<Entity>& parent)
	{
		std::shared_ptr<Entity> newEntity = m_scene->CreateEntity(name, parent);
		MeshRenderer* renderer = newEntity->AddComponent<MeshRenderer>();

		Metadata& meta = AssetRegistry::GetOrCreateMetadata(modelPath);
		MeshImporter::ImportModel(modelPath, meta);
		std::shared_ptr<Mesh> mesh = ResourceManager::GetMesh(meta, 0);
		if (mesh)
			renderer->SetMesh(mesh);

		return newEntity;
	}
	std::shared_ptr<Entity> HierarchyInterface::CreateCanvas(const std::shared_ptr<Entity>& parent)
	{
		std::shared_ptr<Entity> canvas = m_scene->CreateEntity("Canvas", parent);
		MeshRenderer* renderer = canvas->AddComponent<MeshRenderer>();

		std::string modelPath = "assets/models/primitives/plane.fbx";
		Metadata& meta = AssetRegistry::GetOrCreateMetadata(modelPath);
		MeshImporter::ImportModel(modelPath, meta);
		std::shared_ptr<Mesh> mesh = ResourceManager::GetMesh(meta, 0);
		if (mesh)
			renderer->SetMesh(mesh);

		canvas->AddComponent<RectTransform>();
		canvas->GetComponent<RectTransform>()->AnchoredPosition = canvas->GetTransform()->GetLocalPosition();
		canvas->AddComponent<Canvas>();
		canvas->AddComponent<CanvasScaler>();

		return canvas;
	}
	std::shared_ptr<Entity> HierarchyInterface::CreateImage(const std::shared_ptr<Entity>& parent)
	{
		std::shared_ptr<Entity> image = m_scene->CreateEntity("Image", parent);
		MeshRenderer* renderer = image->AddComponent<MeshRenderer>();

		std::string modelPath = "assets/models/primitives/plane.fbx";
		Metadata& meta = AssetRegistry::GetOrCreateMetadata(modelPath);
		MeshImporter::ImportModel(modelPath, meta);
		std::shared_ptr<Mesh> mesh = ResourceManager::GetMesh(meta, 0);
		if (mesh)
			renderer->SetMesh(mesh);

		image->AddComponent<RectTransform>();
		image->AddComponent<Image>();

		return image;
	}
	std::shared_ptr<Entity> HierarchyInterface::CreateButton(const std::shared_ptr<Entity>& parent)
	{
		std::shared_ptr<Entity> button = m_scene->CreateEntity("Button", parent);
		MeshRenderer* renderer = button->AddComponent<MeshRenderer>();

		std::string modelPath = "assets/models/primitives/plane.fbx";
		Metadata& meta = AssetRegistry::GetOrCreateMetadata(modelPath);
		MeshImporter::ImportModel(modelPath, meta);
		std::shared_ptr<Mesh> mesh = ResourceManager::GetMesh(meta, 0);
		if (mesh)
			renderer->SetMesh(mesh);

		button->AddComponent<RectTransform>();
		button->AddComponent<Image>();
		button->AddComponent<Button>();

		return button;
	}
}