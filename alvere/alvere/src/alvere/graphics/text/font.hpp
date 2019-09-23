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

		class Face
		{
		public:

			class Bitmap
			{
			public:

				Bitmap(const char * filepath, unsigned int fontHeightPixels);

				unsigned int getGlyphHeightPixels() const;

				AssetRef<Texture> getTexture() const;

				bool getGlyph(const Glyph * & glyph, unsigned long charCode) const;

			private:

				unsigned int m_glyphHeightPixels;

				Asset<Texture> m_bitmapTexture;

				std::map<unsigned long, Glyph> m_glyphs;

				float m_fontFaceHeight;

				float m_fontFaceMaxAdvance;
			};

			Face(const char * fontFilepath);

			const Bitmap * getBitmap(unsigned int fontHeightPixels);

		private:

			std::string m_name;

			std::string m_resourceFilepath;

			Style m_fontStyle;

			std::vector<Bitmap> m_bitmaps;
		};

	private:

		std::unordered_map<Style, Face> m_faces;
	};
}