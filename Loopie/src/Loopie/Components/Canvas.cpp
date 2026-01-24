#include "Canvas.h"
#include "Loopie/Components/RectTransform.h"
#include "Loopie/Components/CanvasScaler.h"
#include "Loopie/Core/Application.h"
#include "Loopie/Resources/ResourceManager.h"
#include "Loopie/Importers/TextureImporter.h"
#include "Loopie/Importers/MaterialImporter.h"
#include "Loopie/Components/MeshRenderer.h"
#include "Loopie/Core/Log.h"

void Loopie::Canvas::Init()
{
	auto meshRenderer = GetOwner()->GetComponent<MeshRenderer>();
	if (!meshRenderer) return;

	Metadata& uiMatMeta = AssetRegistry::GetOrCreateMetadata("assets/materials/defaultMaterial.mat");
	MaterialImporter::ImportMaterial("assets/materials/defaultMaterial.mat", uiMatMeta);
	std::shared_ptr<Material> templateMaterial = ResourceManager::GetMaterial(uiMatMeta);

	m_material = std::make_shared<Material>(*templateMaterial);
	m_material->SetIfEditable(true);

	if (!pathSprite.empty()) {
		Metadata& spriteMeta = AssetRegistry::GetOrCreateMetadata(pathSprite);
		TextureImporter::ImportImage(pathSprite, spriteMeta);
		Sprite = ResourceManager::GetTexture(spriteMeta);

		if (Sprite) {
			m_material->SetTexture(Sprite);
			Log::Info("Image '{0}': Loaded texture '{1}' (ID: {2})",
				GetOwner()->GetName(),
				pathSprite,
				Sprite ? Sprite->GetRendererId() : 0);
		}
	}

	meshRenderer->SetMaterial(m_material);
}

Loopie::vec2 Loopie::Canvas::GetCanvasSize() const
{
	if (Mode == RenderMode::ScreenSpaceOverlay) {
		auto& app = Application::GetInstance();
		return vec2((float)app.GetWindow().GetSize().x, (float)app.GetWindow().GetSize().y);
	}

	if (auto rt = GetOwner()->GetComponent<RectTransform>()) {
		return rt->GetSize();
	}

	return vec2(100.0f, 100.0f);
}

void Loopie::Canvas::UpdateUIHierarchy()
{
	vec2 currentSize = GetCanvasSize();
	float scaleFactor = 1.0f;

	if (auto scaler = GetOwner()->GetComponent<CanvasScaler>())
	{
		scaleFactor = scaler->GetScaleFactor();
	}

	for (auto& child : GetOwner()->GetChildren())
	{
		if (!child) continue;

		if (auto rt = child->GetComponent<RectTransform>()) {
			rt->UpdateMatrix(currentSize, scaleFactor, this);
		}
	}
}

void Loopie::Canvas::SetVAO(std::shared_ptr<VertexArray> vao)
{
	m_QuadVAO = vao;
}

void Loopie::Canvas::NotifyHierarchyDirty(Entity* parent)
{
	Entity* root = parent ? parent : GetOwner().get();

	for (auto& child : root->GetChildren())
	{
		if (!child) continue;

		if (auto rt = child->GetComponent<RectTransform>()) {
			rt->MarkDirty();
		}

		NotifyHierarchyDirty(child.get());
	}
}

void Loopie::Canvas::SetPixelPerfect(bool enable)
{
	if (PixelPerfect == enable) return;
	PixelPerfect = enable;
	NotifyHierarchyDirty();
}

Loopie::JsonNode Loopie::Canvas::Serialize(JsonNode& parent) const
{
	JsonNode node = parent.CreateObjectField("canvas");
	node.CreateField("renderMode", (int)Mode);
	node.CreateField("pixelPerfect", PixelPerfect);
	return node;
}

void Loopie::Canvas::Deserialize(const JsonNode& data)
{
	Mode = (RenderMode)data.GetValue<int>("renderMode", 0).Result;
	PixelPerfect = data.GetValue<bool>("pixelPerfect", true).Result;
	this->Init();
}
