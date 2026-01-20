#include "Canvas.h"

namespace Loopie {

    Canvas::Canvas() {

    }

    Canvas::~Canvas()
    {

    }

    void Canvas::Init()
    {

    }

    JsonNode Canvas::Serialize(JsonNode& parent) const {

        JsonNode canvas = parent.CreateObjectField("ui_canvas");

        canvas.CreateField("RenderMode", static_cast<int>(mode));
        canvas.CreateField("ReferenceResolutionX", referenceResolution.x);
        canvas.CreateField("ReferenceResolutionY", referenceResolution.y);

        return canvas;
    }

    void Canvas::Deserialize(const JsonNode& data) {}
}
