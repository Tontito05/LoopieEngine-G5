#pragma once
#include "Loopie/Components/Component.h"
#include "Loopie/Math/MathTypes.h"

namespace Loopie{

	class RectTransform;
	class Image;
	class Canvas;

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
		virtual void Update() override;

		void SetState(ButtonState state);
				
		virtual JsonNode Serialize(JsonNode& parent) const override;
		virtual void Deserialize(const JsonNode& data) override;

		ButtonState GetCurrentState() const { return m_currentState; }

	protected:
		void OnClick();
		void OnHoverEnter();
		void OnHoverExit();
		void OnPressed();
		void OnDisabled();
		void OnNormal();

	private:
		RectTransform* m_rectTransform = nullptr;
		Image* image = nullptr;

		ButtonState m_currentState = ButtonState::Normal;
	};
};