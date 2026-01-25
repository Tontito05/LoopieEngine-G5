#include "Font.h"
#include <glad/glad.h>
#include "Loopie/Core/Log.h"

namespace Loopie {
    Font::Font(const std::string& fontPath, unsigned int fontSize) {
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            Log::Critical("Freetype: No se pudo inicializar.");
            return;
        }

        FT_Face face;
        if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
            Log::Critical("Freetype: Error cargando {0}", fontPath);
            return;
        }

        FT_Set_Pixel_Sizes(face, 0, fontSize);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (unsigned char c = 0; c < 128; c++) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;

            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
                0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            m_Characters[c] = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                (unsigned int)face->glyph->advance.x
            };
        }
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        // --- Configuración de Buffers
        m_VAO = new VertexArray();

        // Cada vértice tiene: Posicion(3 floats) + TexCoord(2 floats) = 5 floats
        // Un Quad tiene 4 vértices = 20 floats totales
        m_VBO = new VertexBuffer(nullptr, sizeof(float) * 4 * 5);

        BufferLayout layout;
        layout.AddLayoutElement(0, GLVariableType::FLOAT, 3, "a_Position");
        layout.AddLayoutElement(1, GLVariableType::FLOAT, 2, "a_TexCoord");
        m_VBO->SetLayout(layout);

        unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
        m_EBO = new IndexBuffer(indices, 6);

        m_VAO->AddBuffer(m_VBO, m_EBO);
    }

    void Font::RenderText(Shader& shader, const std::string& text, float x, float y, float scale, const matrix4& transform, const vec4& color) {
        shader.Bind();
        shader.SetUniformVec4("u_Color", color);
        shader.SetUniformMat4("lp_Transform", transform);
        shader.SetUniformInt("u_FontAtlas", 0);

        glActiveTexture(GL_TEXTURE0);
        m_VAO->Bind();

        float cursorX = x;

        for (unsigned char c : text) {
            Character ch = m_Characters[c];

            float xpos = cursorX + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;

            // Datos: { x, y, z, u, v }
            float vertices[4 * 5] = {
                xpos,     ypos + h, 0.0f,   0.0f, 0.0f, // Top-left
                xpos,     ypos,     0.0f,   0.0f, 1.0f, // Bottom-left
                xpos + w, ypos,     0.0f,   1.0f, 1.0f, // Bottom-right
                xpos + w, ypos + h, 0.0f,   1.0f, 0.0f  // Top-right
            };

            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            m_VBO->SetData(vertices, sizeof(vertices));

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            cursorX += (ch.Advance >> 6) * scale;
        }
        m_VAO->Unbind();
    }
}