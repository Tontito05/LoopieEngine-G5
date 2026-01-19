#pragma once
#include "Loopie/Components/Component.h"

namespace Loopie {

    class GUICanvas : public Component {
    public:

        DEFINE_TYPE(GUICanvas)

        enum class RenderMode {
            ScreenSpace,
            WorldSpace
        };

		GUICanvas() = default;

        void Init() override {}

        RenderMode Mode = RenderMode::ScreenSpace;

        vec2 ReferenceResolution = vec2(1920, 1080);

        //Func
        void UpdateCanvasAndChildren(const vec2& windowSize);

        JsonNode Serialize(JsonNode& parent) const override {

            JsonNode canvas = parent.CreateObjectField("ui_canvas");

            canvas.CreateField("RenderMode", static_cast<int>(Mode));
            canvas.CreateField("ReferenceResolutionX", ReferenceResolution.x);
            canvas.CreateField("ReferenceResolutionY", ReferenceResolution.y);

            return canvas;
		}
        void Deserialize(const JsonNode& data) override {}
    };

}
