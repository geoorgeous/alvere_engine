#pragma once

#include <map>
#include <string>
#include <unordered_map>

#include "alvere/assets.hpp"
#include "alvere/utils/shapes.hpp"

namespace alvere
{
	class Texture;

	class Font
	{
	public:

		enum class Style
		{
			Regular,
			Bold,
			Italic,
			BoldItalic
		};

		struct Glyph
		{
			std::string name;

			RectI bitmapSource;

			RectI bounds;

			unsigned int advance;
		};

		class FontFaceBitmap
		{
		public:

			FontFaceBitmap(const char * fontFilepath, unsigned int height);

			FontFaceBitmap(const char * fontFilepath, unsigned int fontWidthPixels, unsigned int fontHeightPixels);

			AssetRef<Texture> getBitmapTexture() const;

			bool getGlyph(const Font::Glyph *& glyph, unsigned int charCode) const;

		private:

			std::string m_name;

			Style m_fontStyle;

			std::map<unsigned long, Glyph> m_glyphs;

			Asset<Texture> m_bitmap;
		};

	private:

		std::unordered_map<Style, Texture> m_bitmaps;
	};
}