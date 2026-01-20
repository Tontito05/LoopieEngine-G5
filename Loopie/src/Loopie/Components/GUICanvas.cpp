#include "GUICanvas.h"

namespace Loopie {

        GUICanvas::GUICanvas() {

        }

		GUICanvas::~GUICanvas() 
        {

		}

        void GUICanvas::Init() 
        {
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
