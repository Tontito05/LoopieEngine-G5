#pragma once
#include "Loopie/Components/Component.h"
#include "Loopie/Math/MathTypes.h"

namespace Loopie
{
	enum class ScaleMode
	{
		ConstantPixelSize,
		ScaleWithScreenSize,
	};

	class CanvasScaler : public Component
	{
	public:
		ScaleMode Mode = ScaleMode::ScaleWithScreenSize;

		vec2 ReferenceResolution{ 1920.0f, 1080.0f };
		
		float MatchWidthOrHeight = 0.5f;

		DEFINE_TYPE(CanvasScaler)
		
		CanvasScaler() = default;

		virtual void Init() override {}

		virtual JsonNode Serialize(JsonNode& parent) const override;
		virtual void Deserialize(const JsonNode& data) override;
	};
}