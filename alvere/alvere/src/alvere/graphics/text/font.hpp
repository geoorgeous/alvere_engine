#pragma once

#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include "alvere/utils/shapes.hpp"
#include "alvere/math/vectors.hpp"

namespace alvere
{
	class Texture;

	class Font
	{
	public:

		enum class Style
		{
			None = -1,
			Regular,
			Bold,
			Italic,
			BoldItalic
		};

		struct Glyph
		{
			std::string name;

			RectI bitmapSource;

			Vector2i bairings;

			Vector2i size;

			unsigned int advance;
		};

		class Face
		{
		public:

			class Bitmap
			{
			public:

				Bitmap(const char * filepath, unsigned int fontHeightPixels);

				inline unsigned int getGlyphHeightPixels() const
				{
					return m_glyphHeightPixels;
				}

				inline Texture * getTexture() const
				{
					return m_bitmapTexture.get();
				}

				const Glyph * getGlyph(unsigned long charCode) const;

				inline float getFontFaceHeight() const
				{
					return m_fontFaceHeight;
				}

				inline float getFontFaceMaxAdvance() const
				{
					return m_fontFaceMaxAdvance;
				}

				Vector2 getTextSize(const std::string & text) const;

				Vector2 getTextAdvance(const std::string & text) const;

			private:

				unsigned int m_glyphHeightPixels;

				std::unique_ptr<Texture> m_bitmapTexture;

				std::map<unsigned long, Glyph> m_glyphs;

				float m_fontFaceHeight;

				float m_fontFaceMaxAdvance;
			};

			Face(const char * fontFilepath);

			~Face();

			inline const std::string & getName() const
			{
				return m_name;
			}

			inline const std::string & getResourceFilepath() const
			{
				return m_resourceFilepath;
			}

			inline Style getStyle() const
			{
				return m_fontStyle;
			}

			const Bitmap * getBitmap(unsigned int fontHeightPixels) const;

		private:

			std::string m_name;

			std::string m_resourceFilepath;

			Style m_fontStyle;

			mutable std::vector<Bitmap *> m_bitmaps;
		};

		const Face * getFontFace(Style style) const;

		void loadFontFaceFile(const char * filepath);

	private:

		std::unordered_map<Style, Face> m_faces;
	};
}