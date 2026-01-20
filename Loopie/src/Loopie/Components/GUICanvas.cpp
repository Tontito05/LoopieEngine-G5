#include "GUICanvas.h"

namespace Loopie {

        GUICanvas::GUICanvas() {

        }

		GUICanvas::~GUICanvas() {
			material.reset();
		}

        void GUICanvas::Init() 
        {
            material = Material::GetDefault();
            material->SetShader(Shader("assets/shaders/GUIShader.shader"));
        }

        JsonNode GUICanvas::Serialize(JsonNode& parent) const {

            JsonNode canvas = parent.CreateObjectField("ui_canvas");

            canvas.CreateField("RenderMode", static_cast<int>(mode));
            canvas.CreateField("ReferenceResolutionX", referenceResolution.x);
            canvas.CreateField("ReferenceResolutionY", referenceResolution.y);

            return canvas;
        }

        void GUICanvas::Deserialize(const JsonNode& data) {}
}
