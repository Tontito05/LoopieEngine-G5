#include "Image.h"
#include "Loopie/Resources/ResourceManager.h"
#include "Loopie/Importers/TextureImporter.h"
#include "Loopie/Importers/MaterialImporter.h"
#include "Loopie/Components/MeshRenderer.h"
#include "Loopie/Helpers/LoopieHelpers.h"
#include "Loopie/Scene/Entity.h"
#include "Loopie/Core/Log.h"

void Loopie::Image::Init()
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

	// Set the color
	UniformValue colorValue;
	colorValue.type = UniformType::UniformType_vec4;
	colorValue.value = Color;
	m_material->SetShaderVariable("u_Color", colorValue);

	meshRenderer->SetMaterial(m_material);
}

void Loopie::Image::SetSprite(const std::string& path)
{
	pathSprite = path;
}

Loopie::JsonNode Loopie::Image::Serialize(JsonNode& parent) const
{
	JsonNode node = parent.CreateObjectField("image");

	node.CreateField("pathSprite", pathSprite);

	JsonNode color = node.CreateObjectField("color");
	color.CreateField("r", Color.x);
	color.CreateField("g", Color.y);
	color.CreateField("b", Color.z);
	color.CreateField("a", Color.w);

	return node;
}

void Loopie::Image::Deserialize(const JsonNode& data)
{
	JsonResult<std::string> resultPath = data.Child("pathSprite").GetValue<std::string>();
	
	if (resultPath.Found)
	{
		pathSprite = resultPath.Result;
		Metadata& meta = AssetRegistry::GetOrCreateMetadata(pathSprite);
		TextureImporter::ImportImage(pathSprite, meta);
		Sprite = ResourceManager::GetTexture(meta);
	}

	JsonNode colorNode = data.Child("color");
	JsonResult<float> r = colorNode.Child("r").GetValue<float>(1.0f);
	JsonResult<float> g = colorNode.Child("g").GetValue<float>(1.0f);
	JsonResult<float> b = colorNode.Child("b").GetValue<float>(1.0f);
	JsonResult<float> a = colorNode.Child("a").GetValue<float>(1.0f);
	
	Color = vec4(r.Result, g.Result, b.Result, a.Result);
}
