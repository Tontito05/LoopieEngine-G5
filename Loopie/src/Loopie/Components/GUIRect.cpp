#include "GUIRect.h"


#include <memory>
namespace Loopie {

	GUIRect::GUIRect() = default;

        //Getters
        vec2 GUIRect::GetAnchorMin() const
        {
            return anchorMin;
        }
        vec2 GUIRect::GetAnchorMax() const
        {
            return anchorMax;
        }
        vec2 GUIRect::GetSize() const
        {
            return size;
        }
        vec2 GUIRect::GetOffset() const
        {
            return offset;
        }
        vec2 GUIRect::GetPivot() const
        {
            return pivot;
        }

        //Setters
        void GUIRect::SetAnchorMin(const vec2& value)
        {
            anchorMin = value;
            UpdateLayout();
        }
        void GUIRect::SetAnchorMax(const vec2& value)
        {
            anchorMax = value;
            UpdateLayout();
        }
        void GUIRect::SetSize(const vec2& value)
        {
            size = value;
            UpdateLayout();
        }
        void GUIRect::SetOffset(const vec2& value)
        {
            offset = value;
            UpdateLayout();
        }
        void GUIRect::SetPivot(const vec2& value)
        {
            pivot = value;
            UpdateLayout();
        }

        //Gizmos
        void GUIRect::RenderGizmo()  {}

        void GUIRect::Init()  {}

        JsonNode GUIRect::Serialize(JsonNode& parent) const 
        {
            JsonNode rect = parent.CreateObjectField("ui_rect");

            rect.CreateField("AnchorMinX", anchorMin.x);
            rect.CreateField("AnchorMinY", anchorMin.y);
            rect.CreateField("AnchorMaxX", anchorMax.x);
            rect.CreateField("AnchorMaxY", anchorMax.y);

            rect.CreateField("PivotX", pivot.x);
            rect.CreateField("PivotY", pivot.y);

            rect.CreateField("SizeX", size.x);
            rect.CreateField("SizeY", size.y);

            rect.CreateField("OffsetX", offset.x);
            rect.CreateField("OffsetY", offset.y);

            return rect;
        }

        void GUIRect::Deserialize(const JsonNode& data)  {}


}
