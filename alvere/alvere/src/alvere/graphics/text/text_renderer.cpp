#include "alvere/graphics/sprite_batcher.hpp"
#include "alvere/graphics/text/text_renderer.hpp"

namespace alvere
{
	void TextRenderer::drawText(SpriteBatcher & spriteBatcher, const AssetRef<Font::Face> fontface, std::string text, Vector2 position, unsigned int size, Vector4 colour)
	{
		const Font::Face::Bitmap * bitmap = fontface->getBitmap(size);
		const Texture * texture = bitmap->getTexture();

		Vector2 pen = {
			position.x,
			position.y,
		};

		Rect destination;

		const Font::Glyph * glyph = nullptr;

		for (int i = 0; i < text.size(); ++i)
		{
			unsigned long c = text[i];

			if (c == '\n')
			{
				pen.x = position.x;
				pen.y -= bitmap->getFontFaceHeight();
				continue;
			}

			if ((glyph = bitmap->getGlyph(c)) == nullptr)
				continue;

			destination.m_x = pen.x + glyph->bairings[0];
			destination.m_y = pen.y - (glyph->size[1] - glyph->bairings[1]);
			destination.m_width = glyph->size[0];
			destination.m_height = glyph->size[1];

			spriteBatcher.Submit(texture, destination, glyph->bitmapSource, colour);

			pen.x += glyph->advance;
		}
	}
}