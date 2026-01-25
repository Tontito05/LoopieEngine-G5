#include "CanvasScaler.h"
#include "Canvas.h"
#include "Camera.h"
#include "Transform.h"
#include "RectTransform.h"
#include "Loopie/Core/Application.h"

void Loopie::CanvasScaler::ApplyScale()
{
	auto rt = GetOwner()->GetComponent<RectTransform>();
	auto canvas = GetOwner()->GetComponent<Canvas>();
	auto camera = Application::GetInstance().GetScene().GetMainCamera();

	if (!rt || !canvas || !camera) return;

	if (canvas->Mode == RenderMode::ScreenSpaceOverlay)
	{
		float windowWidth = (float)Application::GetInstance().GetInputEvent().GetGameSize().x;
		float windowHeight = (float)Application::GetInstance().GetInputEvent().GetGameSize().y;

		float orthoSize = camera->GetOrthoSize();
		float aspect = (float)windowWidth / (float)windowHeight;

		float worldHeight = orthoSize * 2.0f;
		float worldWidth = worldHeight * aspect;

		m_scaleFactor = worldHeight / windowHeight;

		rt->AnchoredPosition = vec3(camera->GetTransform()->GetLocalPosition());
		rt->AnchoredPosition.z = 0.0f;

		GetOwner()->GetTransform()->SetLocalScale(vec3(m_scaleFactor, m_scaleFactor, 1.0f));
	}
}

Loopie::JsonNode Loopie::CanvasScaler::Serialize(JsonNode& parent) const
{
	JsonNode node = parent.CreateObjectField("canvasscaler");
	node.CreateField("scaleMode", (int)Mode);
	node.CreateField("matchWidthOrHeight", MatchWidthOrHeight);
	node.CreateField("scaleFactor", m_scaleFactor);

	JsonNode refRes = node.CreateObjectField("referenceResolution");
	refRes.CreateField("x", ReferenceResolution.x);
	refRes.CreateField("y", ReferenceResolution.y);
	
	return node;
}

void Loopie::CanvasScaler::Deserialize(const JsonNode& data)
{
	JsonNode node = data.Child("canvasscaler");
	if (!node.IsValid()) return;

	Mode = (ScaleMode)node.GetValue<int>("scaleMode", (int)ScaleMode::ScaleWithScreenSize).Result;
	MatchWidthOrHeight = node.GetValue<float>("matchWidthOrHeight", 0.5f).Result;
	m_scaleFactor = node.GetValue<float>("scaleFactor", 1.0f).Result;

	JsonNode refRes = node.Child("referenceResolution");
	if (refRes.IsValid()) {
		ReferenceResolution.x = refRes.GetValue<float>("x", 1920.0f).Result;
		ReferenceResolution.y = refRes.GetValue<float>("y", 1080.0f).Result;
	}
}
