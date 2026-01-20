#include "GUIRender.h"

namespace Loopie
{
    void GUIRender::Init() {
        // 1. Create a simple Quad (0,0 to 1,1)
        // Your engine likely has a mesh utility for this
        uiMesh = CreateQuad();

        // 2. Default UI material if none is set
        if (!material) {
            material = GetDefaultGUI();
        }
    }

    //std::shared_ptr<Mesh> GUIRender::CreateQuad() {

    //    float vertices[] = {
    //        // Pos      // UV
    //        0.0f, 1.0f, 0.0f, 1.0f, // Bottom-left
    //        1.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
    //        1.0f, 0.0f, 1.0f, 0.0f, // Top-right
    //        0.0f, 0.0f, 0.0f, 0.0f  // Top-left
    //    };

    //    unsigned int indices[] = {
    //        0, 1, 2, // First Triangle
    //        2, 3, 0  // Second Triangle
    //    };

    //    return std::make_shared<Mesh>(vertices, indices);
    //}

    std::shared_ptr<Material> GUIRender::GetDefaultGUI() {

        auto mat = std::make_shared<Material>(UUID::Generate());

        mat->SetShader(Shader("assets/shaders/GUIShader.shader"));

        UniformValue whiteColor;
        whiteColor.type = UniformType::UniformType_vec4;
        whiteColor.value = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        mat->SetShaderVariable("u_Color", whiteColor);

        return mat;
    }

}