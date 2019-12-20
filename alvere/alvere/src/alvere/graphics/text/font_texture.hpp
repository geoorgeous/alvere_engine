#pragma once

#include <string>
#include <unordered_map>

#include "alvere/graphics/text/glyph.hpp"
#include "alvere/graphics/texture.hpp"

namespace alvere
{
	class FontTexture
	{
	public:

		FontTexture(const std::string & filepath);

	private:

		std::unordered_map<unsigned int, Glyph> m_loadedGlyphs;

		Texture * m_texture;
	};
}