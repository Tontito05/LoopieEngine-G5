#pragma once
#include "Loopie/Math/MathTypes.h"
#include "Loopie/Resources/Resource.h"
#include <unordered_map>

namespace Loopie {

    struct Glyph
    {
        vec2 size;
        vec2 bearing;
        float advance;
        vec2 uvMin;
        vec2 uvMax;
    };

	class Font
    {
        friend class FontImporter;
        friend class TextRenderer;
    public:
        const Glyph& GetGlyph(char c) const;
        unsigned int GetTextureID() const { return m_textureID; }
        float GetFontSize() const { return m_fontSize; }

    private:
        std::unordered_map<char, Glyph> m_glyphs;
        unsigned int m_textureID = 0;
        float m_fontSize = 0.0f;
    };
};