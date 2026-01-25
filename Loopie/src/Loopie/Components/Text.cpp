#include "Text.h"
#include "Loopie/Core/Application.h"
#include "Loopie/Components/Canvas.h"
#include "Loopie/Core/Log.h"
#include "Loopie/Components/Transform.h"
#include "Loopie/Scene/Entity.h"

void Loopie::Text::Init()
{
	m_shader = new Shader("assets/shaders/TextShader.shader");
	m_font = std::shared_ptr<Font>(new Font("assets/fonts/roboto.ttf", static_cast<unsigned int>(m_fontSize)));
}

void Loopie::Text::Render()
{
	if (!m_font) return;
	if (m_text.empty()) return;

	Transform* transform = GetOwner()->GetComponent<Transform>();
	if (!transform) return;

	// Construir transform final
	matrix4 model = transform->GetLocalToWorldMatrix();

	vec4 color = vec4(m_color, m_alpha);
	m_font->RenderText(*m_shader, m_text, 0.0f, 0.0f, m_fontSize / 32.0f, model, color);
}

// -----------------------------------------
// Serialization
// -----------------------------------------
Loopie::JsonNode Loopie::Text::Serialize(JsonNode& parent) const
{
	JsonNode node = parent.CreateObjectField("text");

	node.CreateField("text", m_text);
	node.CreateField("fontSize", m_fontSize);

	JsonNode colorArr = node.CreateObjectField("color");
	colorArr.CreateField("r", m_color.x);
	colorArr.CreateField("g", m_color.y);
	colorArr.CreateField("b", m_color.z);
	node.CreateField("alpha", m_alpha);

	return node;
}

void Loopie::Text::Deserialize(const JsonNode& data)
{
	JsonResult<std::string> textResult = data.GetValue<std::string>("text", m_text);
	if (textResult.Found) {
		m_text = textResult.Result;
	}
	JsonResult<float> fontSizeResult = data.GetValue<float>("fontSize", m_fontSize);
	if (fontSizeResult.Found) {
		m_fontSize = fontSizeResult.Result;
	}
	JsonNode colorNode = data.Child("color");
	if (colorNode.IsValid()) {
		JsonResult<float> rResult = colorNode.GetValue<float>("r", m_color.x);
		if (rResult.Found) {
			m_color.x = rResult.Result;
		}
		JsonResult<float> gResult = colorNode.GetValue<float>("g", m_color.y);
		if (gResult.Found) {
			m_color.y = gResult.Result;
		}
		JsonResult<float> bResult = colorNode.GetValue<float>("b", m_color.z);
		if (bResult.Found) {
			m_color.z = bResult.Result;
		}
	}
	JsonResult<float> alphaResult = data.GetValue<float>("alpha", m_alpha);
	if (alphaResult.Found) {
		m_alpha = alphaResult.Result;
	}
}