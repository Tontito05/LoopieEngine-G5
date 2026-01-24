#pragma once
#include "Loopie/Components/Component.h"
#include "Loopie/Math/MathTypes.h"

namespace Loopie{

	class RectTransform;
	class Image;

	enum class ButtonState {
		Normal,
		Hovered,
		Pressed,
		Disabled
	};

	class Button : public Component {
	public:
		DEFINE_TYPE(Button)

		Button() = default;

		bool interactive = true;

		vec4 NormalColor = vec4(1.0f);
		vec4 HoveredColor = vec4(0.8f, 0.8f, 0.8f, 1.0f);
		vec4 PressedColor = vec4(0.6f, 0.6f, 0.6f, 1.0f);
		vec4 DisabledColor = vec4(0.3f, 0.3f, 0.3f, 1.0f);
		
		virtual void Init() override;
		void OnUpdate();
				
		virtual JsonNode Serialize(JsonNode& parent) const override;
		virtual void Deserialize(const JsonNode& data) override;

	protected:
		virtual void OnClick();
		virtual void OnHoverEnter();
		virtual void OnHoverExit();
		virtual void OnPressed();

	private:
		RectTransform* m_rectTransform = nullptr;
		Image* image = nullptr;

		ButtonState m_currentState = ButtonState::Normal;
		
		bool IsMouseOver();
	};
};