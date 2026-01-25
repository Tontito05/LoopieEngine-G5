#pragma once
#include <map>
#include <string>
#include <glm/glm.hpp>
#include "Loopie/Render/VertexArray.h"
#include "Loopie/Render/Shader.h"
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Loopie {
    struct Character {
        unsigned int TextureID;
        glm::ivec2   Size;
        glm::ivec2   Bearing;
        unsigned int Advance;
    };

    class Font {
    public:
        Font(const std::string& fontPath, unsigned int fontSize);

        void RenderText(Shader& shader, const std::string& text, float x, float y, float scale, const matrix4& transform, const vec4& color);

    private:
        std::map<char, Character> m_Characters;
        VertexArray* m_VAO;
        VertexBuffer* m_VBO;
        IndexBuffer* m_EBO;
    };
}