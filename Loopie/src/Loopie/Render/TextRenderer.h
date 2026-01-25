#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Loopie/Resources/Types/Font.h"
#include "Loopie/Math/MathTypes.h"
#include "Loopie/Render/VertexArray.h"
#include "Loopie/Render/Shader.h"

namespace Loopie {

	struct TextVertex
	{
		vec3 position;
		vec2 uv;
		vec4 color;
	};

	class TextRenderer
	{
	public:
		TextRenderer();
		~TextRenderer();

		void Init();
		void SetShader(std::shared_ptr<Shader> shader) { m_shader = shader; }

		void RenderText(const std::shared_ptr<Font>& font,
			const std::string& text, const vec3& position,
			const vec4& color = vec4(1.0f), float scale = 1.0f);

		void RenderTextUI(const std::shared_ptr<Font>& font,
			const std::string& text, const vec2& screenPos,
			const vec4& color = vec4(1.0f), float scale = 1.0f);

		void Flush();

	private:
		std::shared_ptr<VertexArray> m_vao;
		std::shared_ptr<VertexBuffer> m_vbo;
		std::shared_ptr<Shader> m_shader;

		std::vector<TextVertex> m_vertices;

		void AddGlyphQuad(const Glyph& glyph, const vec2& pos, float scale, const vec4& color);
	};
};