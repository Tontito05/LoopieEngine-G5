#include "TextRenderer.h"
#include <glad/glad.h>

Loopie::TextRenderer::TextRenderer()
{
}

Loopie::TextRenderer::~TextRenderer()
{
}

void Loopie::TextRenderer::Init()
{
	m_vao = std::make_shared<VertexArray>();
	m_vbo = std::make_shared<VertexBuffer>(nullptr, sizeof(TextVertex) * 6 * 1024);
	
	BufferLayout layout;
	layout.AddLayoutElement(0, GLVariableType::FLOAT, 3, "a_Position");
	layout.AddLayoutElement(2, GLVariableType::FLOAT, 2, "a_TexCoord");
	layout.AddLayoutElement(1, GLVariableType::FLOAT, 4, "a_Color");

	m_vbo->SetLayout(layout);
	m_vao->AddBuffer(m_vbo.get(), nullptr);
}

void Loopie::TextRenderer::RenderText(const std::shared_ptr<Font>& font,
	const std::string& text, const vec3& position,
	const vec4& color, float scale)
{
	vec2 pen(0.0f, 0.0f);

	for (char c : text)
	{
		const Glyph& glyph = font->GetGlyph(c);
		AddGlyphQuad(glyph, pen, scale, color);
		pen.x += glyph.advance * scale;
	}

	Flush();
}

void Loopie::TextRenderer::RenderTextUI(const std::shared_ptr<Font>& font,
	const std::string& text, const vec2& screenPos,
	const vec4& color, float scale)
{
	vec2 pen = screenPos;
	
	for (char c : text)
	{
		const Glyph& glyph = font->GetGlyph(c);
		AddGlyphQuad(glyph, pen, scale, color);
		pen.x += glyph.advance * scale;
	}
	
	Flush();
}

void Loopie::TextRenderer::Flush()
{
	if (m_vertices.empty())
		return;
	
	m_vbo->SetData(m_vertices.data(), sizeof(TextVertex) * m_vertices.size());

	if(m_shader)
		m_shader->Bind();

	m_vao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size()));
	m_vao->Unbind();

	m_vertices.clear();
}

void Loopie::TextRenderer::AddGlyphQuad(const Glyph& glyph, const vec2& pos, float scale, const vec4& color)
{
	float x0 = pos.x + glyph.bearing.x * scale;
	float y0 = pos.y - (glyph.size.y - glyph.bearing.y) * scale;
	float x1 = x0 + glyph.size.x * scale;
	float y1 = y0 + glyph.size.y * scale;

	m_vertices.push_back({ vec3(x0, y0, 0.0f), glyph.uvMin, color });
	m_vertices.push_back({ vec3(x1, y0, 0.0f), vec2(glyph.uvMax.x, glyph.uvMin.y), color });
	m_vertices.push_back({ vec3(x1, y1, 0.0f), glyph.uvMax, color });

	m_vertices.push_back({ vec3(x0, y0, 0.0f), glyph.uvMin, color });
	m_vertices.push_back({ vec3(x1, y1, 0.0f), glyph.uvMax, color });
	m_vertices.push_back({ vec3(x0, y1, 0.0f), vec2(glyph.uvMin.x, glyph.uvMax.y), color });
}