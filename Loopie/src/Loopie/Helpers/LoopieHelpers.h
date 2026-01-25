#pragma once

#include <string>
#include <vector>

#include "Loopie/Render/VertexArray.h"

namespace Loopie {
    namespace Helper {

        inline void GetPathExistingNames(const std::filesystem::path& path, std::vector<std::string>& items)
        {
            items.clear();
            for (const auto& entry : std::filesystem::directory_iterator(path))
            {
                if (entry.is_regular_file()) {
                    items.push_back(entry.path().stem().string());
                }
            }
		}

        inline std::string MakeUniqueName(const std::string& baseName, const std::vector<std::string>& existing, const std::string& sep = "_")
        {
            if (std::find(existing.begin(), existing.end(), baseName) == existing.end()) {
                return baseName;
            }

            int counter = 1;
            std::string candidate;

            while (true) {
                candidate = baseName + sep + std::to_string(counter);

                if (std::find(existing.begin(), existing.end(), candidate) == existing.end()) {
                    return candidate;
                }

                counter++;
            }
        }

        static void CreateRectBorder(std::shared_ptr<VertexArray>& m_vao)
        {
            float canvasVertices[] = {
                -0.5f, -0.5f,  0.0f,  0.0f, 0.0f,
                 0.5f, -0.5f,  0.0f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.0f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.0f,  0.0f, 1.0f
            };

            unsigned int canvasIndices[] = {
                0, 1, 2,
                2, 3, 0
            };

            m_vao = std::make_shared<VertexArray>();

            auto vbo = new VertexBuffer(canvasVertices, sizeof(canvasVertices));
            
            BufferLayout layout;
            layout.AddLayoutElement(0, Loopie::GLVariableType::FLOAT, 3, "a_Position");
            layout.AddLayoutElement(1, Loopie::GLVariableType::FLOAT, 2, "a_TexCoord");

			vbo->SetLayout(layout);
            auto ebo = new IndexBuffer(canvasIndices, 6);

            m_vao->AddBuffer(vbo, ebo);
        }
    }
}