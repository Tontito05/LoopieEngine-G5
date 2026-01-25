#pragma once

#include <memory>
#include <string>

#include "Loopie/Resources/Types/Font.h"

namespace Loopie {

	class FontImporter 
	{
	public:
		static std::shared_ptr<Font> Import(const std::string& filepath, float pixelHeight);

	private:
		static unsigned int CreateTexture(const unsigned char* data, int width, int height);

		static std::vector<unsigned char> ReadBinaryFile(const std::string& filepath);
	};
}