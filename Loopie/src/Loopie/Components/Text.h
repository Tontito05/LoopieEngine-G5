#pragma once
#include "Loopie/Components/Component.h"
#include "Loopie/Math/MathTypes.h"
#include "Loopie/Resources/Types/Font.h"

namespace Loopie
{
	class Text : public Component
	{
	public:
		DEFINE_TYPE(Text);

		Text() = default;

		virtual void Init() override;

		void Render();

		//Setters
		void SetText(const std::string& text) { m_text = text; }
		void SetFont(const std::shared_ptr<Font>& font) { m_font = font; }
		void SetFontSize(float size) { m_fontSize = size; }
		void SetColor(const vec3& color) { m_color = color; }
		void SetAlpha(float alpha) { m_alpha = alpha; }

		// Getters
		const std::string& GetText() const { return m_text; }
		float GetFontSize() const { return m_fontSize; }
		const vec3& GetColor() const { return m_color; }
		std::shared_ptr<Font> GetFont() const { return m_font; }
		const float GetAlpha() const { return m_alpha; }

		// Serialize & Deserialize
		virtual JsonNode Serialize(JsonNode& parent) const override;
		virtual void Deserialize(const JsonNode& data) override;
		
		bool interactable = false;

	private:
		std::string m_text = "Text";
		std::shared_ptr<Font> m_font = nullptr;

		float m_fontSize = 32.0f;
		vec3 m_color = vec3(1.0f, 1.0f, 1.0f);
		float m_alpha = 1.0f;
		Shader* m_shader = nullptr;

		bool m_dirty = true;
	};
}