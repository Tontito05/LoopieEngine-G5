#include "Canvas.h"
#include "Loopie/Components/RectTransform.h"
#include "Loopie/Components/CanvasScaler.h"
#include "Loopie/Core/Application.h"
#include "Loopie/Resources/ResourceManager.h"
#include "Loopie/Importers/TextureImporter.h"
#include "Loopie/Importers/MaterialImporter.h"
#include "Loopie/Components/MeshRenderer.h"
#include "Loopie/Core/Log.h"

#include <imgui.h>

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

	//if(Mode == RenderMode::ScreenSpaceOverlay)
	//{
	//	auto canvasScaler = GetOwner()->AddComponent<CanvasScaler>();
	//	if (canvasScaler)
	//		canvasScaler->ApplyScale();
	//}
}

void Loopie::Canvas::SetPixelPerfect(bool enable)
{
	if (PixelPerfect == enable) return;
	PixelPerfect = enable;
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
}
