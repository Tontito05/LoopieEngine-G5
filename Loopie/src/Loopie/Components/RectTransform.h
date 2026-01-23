#pragma once
#include "Loopie/Components/Component.h"
#include "Loopie/Math/MathTypes.h"

namespace Loopie
{
	class Canvas;
	class RectTransform : public Component
	{
	public:
		vec3 AnchoredPosition = vec3(0.0f);
		float Width = 1.0f;
		float Height = 1.0f;

		DEFINE_TYPE(RectTransform)

		RectTransform() = default;

		virtual void Init() override { MarkDirty(); }

		// Getters
		const matrix4& GetLocalMatrix() const {return m_LocalToWorldMatrix;}
		vec2 GetSize() const { return vec2(Width, Height); }

		void MarkDirty() { m_Dirty = true; }

		void UpdateMatrix(const vec2& parentSize, float scaleFactor, Canvas* root);

		void RefreshMatrix();

		virtual JsonNode Serialize(JsonNode& parent) const override;
		virtual void Deserialize(const JsonNode& data) override;

		Canvas* FindRootCanvas();

	private:
		vec2 GetParentSize();

		matrix4 m_LocalToWorldMatrix{ 1.0f };
		vec2 Pivot = vec2(0.5f, 0.5f);
		bool m_Dirty = true;
	};
}