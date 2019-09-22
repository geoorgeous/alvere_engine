#include "alvere/graphics/text/font.hpp"

#include <algorithm>
#include <vector>

#include <freetype/ft2build.h>
#include FT_FREETYPE_H  
#include <freetype/freetype/ftbitmap.h>

#include "alvere/graphics/texture.hpp"
#include "alvere/utils/shapes.hpp"
#include "alvere/utils/logging.hpp"

namespace alvere
{
	struct GlyphInternal : Font::Glyph
	{
		GlyphInternal(FT_UInt glyphIndex, FT_ULong charcode, std::string name, RectI bitmapSource, RectI bounds, unsigned int advance)
		{
			this->glyphIndex = glyphIndex;
			this->charcode = charcode;
			this->name = name;
			this->bitmapSource = bitmapSource;
			this->bounds = bounds;
			this->advance = advance;
		}

		FT_UInt glyphIndex;

		FT_ULong charcode;
	};

	Font::FontFaceBitmap::FontFaceBitmap(const char * fontFilepath, unsigned int fontWidthPixels, unsigned int fontHeightPixels)
		: m_fontStyle(Font::Style::Regular), m_bitmap(nullptr)
	{
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
			LogInfo("Could not init FreeType Library\n");

		FT_Face face;
		if (FT_New_Face(ft, fontFilepath, fontWidthPixels, &face))
			LogInfo("Failed to load font\n");

		m_name = std::string(face->family_name) + " " + face->style_name;

		if (face->style_flags & FT_STYLE_FLAG_BOLD && face->style_flags & FT_STYLE_FLAG_ITALIC)
			m_fontStyle = Font::Style::BoldItalic;
		else if (face->style_flags & FT_STYLE_FLAG_BOLD)
			m_fontStyle = Font::Style::Bold;
		else if (face->style_flags & FT_STYLE_FLAG_ITALIC)
			m_fontStyle = Font::Style::Italic;

		FT_Set_Pixel_Sizes(face, fontWidthPixels, fontHeightPixels);

		std::vector<GlyphInternal> sortedGlyphs;
		sortedGlyphs.reserve(face->num_glyphs);

		FT_UInt glyphIndex = 0;
		char glyphName[128];

		for (unsigned char c = 0; c < 128; ++c)
		{
			FT_ULong charcode = (FT_ULong)c;

			glyphIndex = FT_Get_Char_Index(face, (FT_ULong)c);

			if (glyphIndex == 0)
				continue;

			FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);

			FT_Get_Glyph_Name(face, glyphIndex, glyphName, 128);

			// new glyph
			sortedGlyphs.emplace_back(
					glyphIndex,
					charcode,
					glyphName,
					RectI{
						0,
						0,
						(int)face->glyph->bitmap.width,
						(int)face->glyph->bitmap.rows },
					RectI{ // glyph bounds
						face->glyph->bitmap_left,
						face->glyph->bitmap_top,
						(int)face->glyph->bitmap.width,
						(int)face->glyph->bitmap.rows },
					face->glyph->advance.x);
		}

		// sort the glyphs from largest to smallest
		std::sort(sortedGlyphs.begin(), sortedGlyphs.end(),
			[](const GlyphInternal & lhs, const GlyphInternal & rhs) -> bool
		{
			return lhs.bounds.getArea() > rhs.bounds.getArea();
		});

		bool glyphPlacementSuccess = false;
		int maxX = 512;
		int maxY = 1;

		// Step through each glyph that hasn't been placed yet...

		for (int i = 0; i < sortedGlyphs.size(); ++i)
		{
			GlyphInternal & glyphPlacing = sortedGlyphs[i];
			glyphPlacementSuccess = false;

			// Step through each row of pixels... from left to right.
			// Check to see if we tried to place the glyph here would we intersect with any previously placed glyphs?
			// As soon as we find an intersecting glyph then set x to the max x of the existing glyph as to skip a bunch of pixels we know are taken
			// As soon as we find a spot where we can place the glyph we place it down and repeat.

			for (int y = 0; y < maxY && !glyphPlacementSuccess; ++y)
			{
				for (int x = 0; x < maxX && !glyphPlacementSuccess; ++x)
				{
					// For every pixel, see if it's available

					// Must proceed to next row if we've run out of room on this row

					if (x + glyphPlacing.bitmapSource.m_width > maxX)
						break;

					bool intersects = false;

					for (int j = 0; j < i && !intersects; ++j)
					{
						const GlyphInternal & glyphPlaced = sortedGlyphs[j];

						// check if they would intersect

						RectI candidateBounds{ x, y, glyphPlacing.bitmapSource.m_width, glyphPlacing.bitmapSource.m_height };

						if (candidateBounds.intersects(glyphPlaced.bitmapSource))
						{
							x = glyphPlaced.bitmapSource.m_x + glyphPlaced.bitmapSource.m_width;
							intersects = true;
						}
					}

					if (intersects)
						continue; // bad placement, we've found intersection for this spot

					// good placement

					glyphPlacing.bitmapSource.m_x = x;
					glyphPlacing.bitmapSource.m_y = y;
					glyphPlacementSuccess = true;

					// Resize the height of the bitmap if the new placed glyph overflows the bottom

					if (glyphPlacing.bitmapSource.m_y + glyphPlacing.bitmapSource.m_height > maxY)
					{
						maxY = glyphPlacing.bitmapSource.m_y + glyphPlacing.bitmapSource.m_height;
					}

					m_glyphs[glyphPlacing.charcode] = (Glyph)glyphPlacing;
				}
			}
		}

		// Now all glyphs are placed, it's time to generate the texture data.

		unsigned char * pixels = new unsigned char[(size_t)maxX * maxY]();

		const GlyphInternal * glyph;
		FT_Bitmap * bitmap;
		unsigned int xOffset = 0;
		unsigned int yOffset = 0;

		for (int i = 0; i < sortedGlyphs.size(); ++i)
		{
			glyph = &sortedGlyphs[i];

			FT_Load_Glyph(face, glyph->glyphIndex, FT_LOAD_RENDER);

			bitmap = &face->glyph->bitmap;

			xOffset = glyph->bitmapSource.m_x;

			for (unsigned int y = 0; y < bitmap->rows; ++y)
			{
				yOffset = (glyph->bitmapSource.m_y + y) * maxX;

				std::memcpy(&pixels[yOffset + xOffset], &bitmap->buffer[bitmap->pitch * y], bitmap->pitch);
			}
		}

		m_bitmap = Texture::New(pixels, maxX, maxY, Texture::Channels::Grey);

		delete[] pixels;

		FT_Done_Face(face);

		LogInfo("Successfully created Font bitmap texture '%s'\n", m_name.c_str());
		LogContinue("\tFont line height (pixels): %i\n", fontHeightPixels);
		LogContinue("\tDimensions: %i x %i\n", m_bitmap->width(), m_bitmap->height());
		LogContinue("\tSize: %.3fKB\n", (float)(m_bitmap->width() * m_bitmap->height()) / 1024.0f);
	}

	Font::FontFaceBitmap::FontFaceBitmap(const char * fontFilepath, unsigned int fontHeightPixels)
		: Font::FontFaceBitmap::FontFaceBitmap(fontFilepath, 0, fontHeightPixels)
	{ }

	AssetRef<Texture> Font::FontFaceBitmap::getBitmapTexture() const
	{
		return m_bitmap.get();
	}

	bool Font::FontFaceBitmap::getGlyph(const Font::Glyph * & glyph, unsigned int charCode) const
	{
		auto iter = m_glyphs.find(charCode);

		if (iter == m_glyphs.end())
		{
			glyph = nullptr;
			return false;
		}

		glyph = &iter->second;
		return true;
	}
}