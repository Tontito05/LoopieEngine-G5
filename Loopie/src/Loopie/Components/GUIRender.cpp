#include "GUIRender.h"
#include "Loopie/Resources/AssetRegistry.h"
#include "Loopie/Importers/MeshImporter.h"
#include "Loopie/Resources/ResourceManager.h"


namespace Loopie
{
    void GUIRender::Init() {

        uiMesh = CreateQuad();

        if (!material) {
            material = GetDefaultGUI();
        }
    }

    GUIRender::GUIRender() {
        
	}

	GUIRender::~GUIRender() {
	}

    std::shared_ptr<Mesh> GUIRender::CreateQuad() {

        Metadata& meta = AssetRegistry::GetOrCreateMetadata("assets/models/primitives/plane.fbx");
        MeshImporter::ImportModel("assets/models/primitives/plane.fbx", meta);
        std::shared_ptr<Mesh> mesh = ResourceManager::GetMesh(meta, 0);

        return mesh;
    }

    std::shared_ptr<Material> GUIRender::GetDefaultGUI() {

        std::shared_ptr<Material> mat = Material::GetDefault();

        mat->SetShader(Shader("assets/shaders/GUIShader.shader"));

        UniformValue whiteColor;
        whiteColor.type = UniformType::UniformType_vec4;
        whiteColor.value = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        mat->SetShaderVariable("u_Color", whiteColor);

        return mat;
    }

    JsonNode GUIRender::Serialize(JsonNode& parent) const {
        JsonNode guiRenderObj = parent.CreateObjectField("GUIRender");
        if (material) {
            guiRenderObj.CreateField<std::string>("Material", material->GetUUID().Get());
        }
		return guiRenderObj;
	}

    void GUIRender::Deserialize(const JsonNode& data) {
        if (data.Contains("Material")) {
            UUID matUUID = UUID(data.Child("Material").GetValue<std::string>().Result);
            Metadata* meta = AssetRegistry::GetMetadata(matUUID);
            if (meta) {
                material = ResourceManager::GetMaterial(*meta);
            }
        }
	}

}