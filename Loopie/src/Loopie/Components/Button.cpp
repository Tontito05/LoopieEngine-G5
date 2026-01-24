#include "Button.h"
#include "Loopie/Scene/Entity.h"
#include "Loopie/Components/RectTransform.h"
#include "Loopie/Components/Image.h"
#include "Loopie/Core/Application.h"
#include "Loopie/Core/InputEventManager.h"
#include "Loopie/Core/Log.h"

void Loopie::Button::Init()
{
	m_rectTransform = GetOwner()->GetComponent<RectTransform>();
	if (!m_rectTransform) {
		Log::Warn("Button component requires a RectTransform component on the same entity: '{0}'",
			GetOwner()->GetName());
	}

	image = GetOwner()->GetComponent<Image>();
}

void Loopie::Button::OnUpdate()
{
	if (!m_rectTransform) return;

	InputEventManager& inputEvent = Application::GetInstance().GetInputEvent();

	ButtonState wasHovered = m_currentState;

	if (!interactive) {
		m_currentState = ButtonState::Disabled;
		if (image) {
			image->SetColor(DisabledColor);
		}
		return;
	}
	else m_currentState = IsMouseOver() ? ButtonState::Hovered : ButtonState::Normal;

	if(m_currentState == ButtonState::Hovered && wasHovered != ButtonState::Hovered)
	{
		OnHoverEnter();
	}
	else if(m_currentState != ButtonState::Hovered && wasHovered == ButtonState::Hovered)
	{
		OnHoverExit();
	}

	if (m_currentState == ButtonState::Hovered) {
		if (inputEvent.GetMouseButtonStatus(SDL_BUTTON_LEFT) == KeyState::DOWN) {
			m_currentState = ButtonState::Pressed;
			OnPressed();
		}
		else if (inputEvent.GetMouseButtonStatus(SDL_BUTTON_LEFT) == KeyState::UP) {
			m_currentState = ButtonState::Normal;
			OnClick();
		}
	}

	if (m_currentState == ButtonState::Normal) {
		if (image) {
			image->SetColor(NormalColor);
		}
	}
}

Loopie::JsonNode Loopie::Button::Serialize(JsonNode& parent) const
{
	JsonNode node = parent.CreateObjectField("button");

	node.CreateField("interactive", interactive);

	JsonNode normalColor = node.CreateObjectField("normalColor");
	normalColor.CreateField("r", NormalColor.r);
	normalColor.CreateField("g", NormalColor.g);
	normalColor.CreateField("b", NormalColor.b);
	normalColor.CreateField("a", NormalColor.a);

	JsonNode hoveredColor = node.CreateObjectField("hoveredColor");
	hoveredColor.CreateField("r", HoveredColor.r);
	hoveredColor.CreateField("g", HoveredColor.g);
	hoveredColor.CreateField("b", HoveredColor.b);
	hoveredColor.CreateField("a", HoveredColor.a);

	JsonNode pressedColor = node.CreateObjectField("pressedColor");
	pressedColor.CreateField("r", PressedColor.r);
	pressedColor.CreateField("g", PressedColor.g);
	pressedColor.CreateField("b", PressedColor.b);
	pressedColor.CreateField("a", PressedColor.a);

	JsonNode disabledColor = node.CreateObjectField("disabledColor");
	disabledColor.CreateField("r", DisabledColor.r);
	disabledColor.CreateField("g", DisabledColor.g);
	disabledColor.CreateField("b", DisabledColor.b);
	disabledColor.CreateField("a", DisabledColor.a);

	return node;
}

void Loopie::Button::Deserialize(const JsonNode& data)
{
	if (!data.IsValid()) return;

	interactive = data.GetValue<bool>("interactive", true).Result;

	JsonNode normalColor = data.Child("normalColor");
	if (normalColor.IsValid()) {
		NormalColor.r = normalColor.GetValue<float>("r", 1.0f).Result;
		NormalColor.g = normalColor.GetValue<float>("g", 1.0f).Result;
		NormalColor.b = normalColor.GetValue<float>("b", 1.0f).Result;
		NormalColor.a = normalColor.GetValue<float>("a", 1.0f).Result;
	}
	JsonNode hoveredColor = data.Child("hoveredColor");
	if (hoveredColor.IsValid()) {
		HoveredColor.r = hoveredColor.GetValue<float>("r", 0.8f).Result;
		HoveredColor.g = hoveredColor.GetValue<float>("g", 0.8f).Result;
		HoveredColor.b = hoveredColor.GetValue<float>("b", 0.8f).Result;
		HoveredColor.a = hoveredColor.GetValue<float>("a", 1.0f).Result;
	}
	JsonNode pressedColor = data.Child("pressedColor");
	if (pressedColor.IsValid()) {
		PressedColor.r = pressedColor.GetValue<float>("r", 0.6f).Result;
		PressedColor.g = pressedColor.GetValue<float>("g", 0.6f).Result;
		PressedColor.b = pressedColor.GetValue<float>("b", 0.6f).Result;
		PressedColor.a = pressedColor.GetValue<float>("a", 1.0f).Result;
	}
	JsonNode disabledColor = data.Child("disabledColor");
	if (disabledColor.IsValid()) {
		DisabledColor.r = disabledColor.GetValue<float>("r", 0.3f).Result;
		DisabledColor.g = disabledColor.GetValue<float>("g", 0.3f).Result;
		DisabledColor.b = disabledColor.GetValue<float>("b", 0.3f).Result;
		DisabledColor.a = disabledColor.GetValue<float>("a", 1.0f).Result;
	}
}

void Loopie::Button::OnClick()
{
	Log::Info("Button '{0}' clicked!", GetOwner()->GetName());

	if (image) {
		image->SetColor(PressedColor);
	}
}

void Loopie::Button::OnHoverEnter()
{
	Log::Info("Button '{0}' hover enter!", GetOwner()->GetName());

	if (image) {
		image->SetColor(HoveredColor);
	}
}

void Loopie::Button::OnHoverExit()
{
	Log::Info("Button '{0}' hover exit!", GetOwner()->GetName());
	if (image) {
		image->SetColor(NormalColor);
	}
}

void Loopie::Button::OnPressed()
{
	Log::Info("Button '{0}' pressed!", GetOwner()->GetName());
	if (image) {
		image->SetColor(PressedColor);
	}
}

bool Loopie::Button::IsMouseOver()
{
	auto rt = GetOwner()->GetComponent<RectTransform>();
	if (!rt) return false;

	const matrix4& model = rt->GetLocalMatrix();
	vec2 pos = vec2(model[3][0], model[3][1]);
	vec2 size = rt->GetSize();

	InputEventManager& inputEvent = Application::GetInstance().GetInputEvent();
	vec2 mousePos = inputEvent.GetMousePosition();

	return (mousePos.x >= pos.x && mousePos.x <= pos.x + size.x &&
		mousePos.y >= pos.y && mousePos.y <= pos.y + size.y);
}
