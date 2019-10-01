#include "alvere/graphics/text/font.hpp"

#include <algorithm>
#include <vector>

#include <freetype/ft2build.h>
#include FT_FREETYPE_H  
#include <freetype/freetype/ftbitmap.h>
#include <freetype/freetype/ftmodapi.h>

#include "alvere/graphics/texture.hpp"
#include "alvere/utils/shapes.hpp"
#include "alvere/utils/logging.hpp"

namespace alvere
{
	struct GlyphInternal : Font::Glyph
	{
		GlyphInternal(FT_UInt glyphIndex, FT_ULong charcode, std::string name, RectI bitmapSource, Vector2i bairings, Vector2i size, unsigned int advance)
		{
			this->glyphIndex = glyphIndex;
			this->charcode = charcode;
			this->name = name;
			this->bitmapSource = bitmapSource;
			this->bairings = bairings;
			this->size = size;
			this->advance = advance;
		}

		FT_UInt glyphIndex;

		FT_ULong charcode;
	};

	Font::Face::Bitmap::Bitmap(const char * filepath, unsigned int fontHeightPixels)
	{
		FT_Library ft;

		if (FT_Init_FreeType(&ft))
		{
			LogError("Failed to initialise font loading library!\n");
			return;
		}

		FT_Face face;

		if (FT_New_Face(ft, filepath, 0, &face))
		{
			LogWarning("Failed to load font\n");
			return;
		}

		m_glyphHeightPixels = fontHeightPixels;

		FT_Set_Pixel_Sizes(face, 0, fontHeightPixels);

		const unsigned char charCount = 128;

		std::vector<GlyphInternal> sortedGlyphs;
		sortedGlyphs.reserve(charCount);

		FT_ULong charcode = 0;;
		FT_UInt glyphIndex = 0;
		char glyphName[charCount];

		for (unsigned char c = 0; c < charCount; ++c)
		{
			charcode = (FT_ULong)c;

			glyphIndex = FT_Get_Char_Index(face, charcode);

			if (glyphIndex == 0)
				continue;

			FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);

			FT_Get_Glyph_Name(face, glyphIndex, glyphName, 128);

			// new glyph
			sortedGlyphs.emplace_back(
				glyphIndex,
				charcode,
				glyphName,
				RectI{ 0, 0, (int)face->glyph->bitmap.width, (int)face->glyph->bitmap.rows },
				Vector2i{ face->glyph->bitmap_left, face->glyph->bitmap_top },
				Vector2i{ (int)face->glyph->bitmap.width, (int)face->glyph->bitmap.rows },
				face->glyph->advance.x >> 6);
		}

		// sort the glyphs from largest to smallest
		std::sort(sortedGlyphs.begin(), sortedGlyphs.end(),
			[](const GlyphInternal & lhs, const GlyphInternal & rhs) -> bool
			{
				return lhs.size[0] * lhs.size[1] > rhs.size[0] * lhs.size[1];
			});

		GlyphInternal * glyphPlacing = nullptr;
		GlyphInternal * placedGlyph = nullptr;
		RectI potentialBounds;
		bool intersects = false;
		bool glyphPlacementSuccess = false;
		int maxX = 512;
		int maxY = 1;

		// Step through each glyph that hasn't been placed yet...

		for (int i = 0; i < sortedGlyphs.size(); ++i)
		{
			glyphPlacing = &sortedGlyphs[i];
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

					if (x + glyphPlacing->bitmapSource.m_width > maxX)
						break;

					intersects = false;

					for (int j = 0; j < i && !intersects; ++j)
					{
						placedGlyph = &sortedGlyphs[j];

						// check if they would intersect

						potentialBounds.m_x = x;
						potentialBounds.m_y = y;
						potentialBounds.m_width = glyphPlacing->bitmapSource.m_width;
						potentialBounds.m_height = glyphPlacing->bitmapSource.m_height;

						if (potentialBounds.intersects(placedGlyph->bitmapSource))
						{
							x = placedGlyph->bitmapSource.m_x + placedGlyph->bitmapSource.m_width;
							intersects = true;
						}
					}

					if (intersects)
						continue; // bad placement, we've found intersection for this spot

					// good placement

					glyphPlacing->bitmapSource.m_x = x;
					glyphPlacing->bitmapSource.m_y = y;
					glyphPlacementSuccess = true;

					// Resize the height of the bitmap if the new placed glyph overflows the bottom

					if (glyphPlacing->bitmapSource.m_y + glyphPlacing->bitmapSource.m_height > maxY)
					{
						maxY = glyphPlacing->bitmapSource.m_y + glyphPlacing->bitmapSource.m_height;
					}

					m_glyphs[glyphPlacing->charcode] = (Glyph)* glyphPlacing;
				}
			}
		}

		// Now all glyphs are placed, it's time to generate the texture data.

		unsigned char * pixels = new unsigned char[(size_t)maxX * maxY]();

		const GlyphInternal * glyph;
		const FT_Bitmap * bitmap;
		unsigned int xOffset = 0;
		unsigned int yOffset = 0;

		for (int i = 0; i < sortedGlyphs.size(); ++i)
		{
			glyph = &sortedGlyphs[i];

			FT_Load_Glyph(face, glyph->glyphIndex, FT_LOAD_RENDER);

			bitmap = &face->glyph->bitmap;

			xOffset = glyph->bitmapSource.m_x;

			m_glyphs[glyph->charcode].bitmapSource.m_y = (maxY) - glyph->bitmapSource.m_y - glyph->bitmapSource.m_height;

			for (unsigned int y = 0; y < bitmap->rows; ++y)
			{
				yOffset = (maxY - 1) - (glyph->bitmapSource.m_y + y);

				std::memcpy(&pixels[yOffset * maxX + xOffset], &bitmap->buffer[bitmap->pitch * y], bitmap->pitch);
			}
		}

		m_bitmapTexture = Texture::New(pixels, maxX, maxY, Texture::Channels::Grey);

		delete[] pixels;

		m_fontFaceHeight = face->size->metrics.height >> 6;
		m_fontFaceMaxAdvance = face->size->metrics.max_advance >> 6;

		FT_Done_Face(face);

		FT_Done_Library(ft);
	}

	const Font::Glyph * Font::Face::Bitmap::getGlyph(unsigned long charCode) const
	{
		auto iter = m_glyphs.find(charCode);

		if (iter == m_glyphs.end())
			return nullptr;

		return &iter->second;
	}

	Font::Face::Face(const char * fontFilepath)
		: m_name("INVALID FONT"), m_resourceFilepath(fontFilepath), m_fontStyle(Font::Style::Regular)
	{
		FT_Library ft;

		if (FT_Init_FreeType(&ft))
		{
			LogError("Failed to initialise font loading library!\n");
			return;
		}

		FT_Face face;

		if (FT_New_Face(ft, fontFilepath, 0, &face))
		{
			LogWarning("Failed to open font file '%s'.\n", fontFilepath);
			return;
		}

		m_name = std::string(face->family_name) + " " + face->style_name;

		if (face->style_flags & FT_STYLE_FLAG_BOLD && face->style_flags & FT_STYLE_FLAG_ITALIC)
			m_fontStyle = Font::Style::BoldItalic;
		else if (face->style_flags & FT_STYLE_FLAG_BOLD)
			m_fontStyle = Font::Style::Bold;
		else if (face->style_flags & FT_STYLE_FLAG_ITALIC)
			m_fontStyle = Font::Style::Italic;

		FT_Done_Face(face);

		FT_Done_Library(ft);
	}

	const Font::Face::Bitmap * Font::Face::getBitmap(unsigned int fontHeightPixels)
	{
		for (const Bitmap & bitmap : m_bitmaps)
			if (bitmap.getGlyphHeightPixels() == fontHeightPixels)
				return &bitmap;

		m_bitmaps.emplace_back(m_resourceFilepath.c_str(), fontHeightPixels);

		if (m_bitmaps.back().getGlyphHeightPixels() == 0)
		{
			m_bitmaps.pop_back();
			return nullptr;
		}

		return &m_bitmaps.back();
	}
}