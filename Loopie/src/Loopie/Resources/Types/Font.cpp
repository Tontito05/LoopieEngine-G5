#include "Font.h"
#include "Loopie/Core/Log.h"
#include "Loopie/Resources/ResourceManager.h"
#include "Loopie/Importers/TextImporter.h"

#include <glad/glad.h>
#include <fstream>
#include <vector>

const Loopie::Glyph& Loopie::Font::GetGlyph(char c) const
{
	static Glyph empty{};
	auto it = m_glyphs.find(c);
	if (it != m_glyphs.end()) {
		return empty;
	}
	return it->second;
}
