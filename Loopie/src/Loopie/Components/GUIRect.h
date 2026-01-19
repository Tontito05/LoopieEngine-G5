#pragma once
#include "Loopie/Components/Component.h"
#include "Loopie/Scene/Entity.h"
#include "Loopie/Math/MathTypes.h"

#include <memory>
namespace Loopie {

    class GUIRect : public Component {
    private:

        vec2 anchorMin = vec2(0, 0);
        vec2 anchorMax = vec2(1, 1);

        vec2 pivot = vec2(0.5f, 0.5f);

        vec2 size = vec2(100, 100);
        vec2 offset = vec2(0, 0); // Extra position offset

	public:

        DEFINE_TYPE(GUIRect)
        GUIRect();

        //Getters
        vec2 GetAnchorMin() const;
        vec2 GetAnchorMax() const;
        vec2 GetSize() const;
        vec2 GetOffset() const;
        vec2 GetPivot() const;


        //Setters
        void SetAnchorMin(const vec2& value);
        void SetAnchorMax(const vec2& value);
        void SetSize(const vec2& value);
        void SetOffset(const vec2& value);
        void SetPivot(const vec2& value);


        //Gizmos
        void RenderGizmo() override;

        void Init() override;

        void UpdateLayout()
        {
            if (GetOwner()->GetComponent<GUIRect>() == nullptr)
                return;

            const vec2& parentSize = GetOwner()->GetComponent<GUIRect>()->GetSize();

            vec2 anchorPos = parentSize * anchorMin;
            vec2 anchorSize = parentSize * (anchorMax - anchorMin);

            vec2 finalSize = anchorSize + size;
            vec2 pos = anchorPos + (finalSize * pivot) + offset;
        }

        JsonNode Serialize(JsonNode& parent) const override;

        void Deserialize(const JsonNode& data) override;
    };

}
