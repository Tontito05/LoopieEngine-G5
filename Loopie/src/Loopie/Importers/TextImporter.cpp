#include "TextImporter.h"

#include <fstream>
#include <vector>

#include <glad/glad.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

std::shared_ptr<Loopie::Font> Loopie::FontImporter::Import(const std::string& filepath, float pixelHeight)
{
	return std::shared_ptr<Font>();
}

unsigned int Loopie::FontImporter::CreateTexture(const unsigned char* data, int width, int height)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_ONE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_ONE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_ONE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);

	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}

std::vector<unsigned char> Loopie::FontImporter::ReadBinaryFile(const std::string& filepath)
{
	std::ifstream file(filepath, std::ios::binary);
	if (!file) {
		return {};
	}

	file.seekg(0, std::ios::end);
	size_t fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<unsigned char> buffer(fileSize);
	file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
	return buffer;
}
