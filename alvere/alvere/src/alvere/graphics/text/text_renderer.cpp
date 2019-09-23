#include "alvere/graphics/sprite_batcher.hpp"
#include "alvere/graphics/text/text_renderer.hpp"

namespace alvere
{
	void TextRenderer::drawText(SpriteBatcher & spriteBatcher, const AssetRef<Font::Face> fontface, std::string text, Vector2 position, float lineHeight, Vector4 colour)
	{
		const Font::Face::Bitmap * bitmap = fontface->getBitmap(48);
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

			if (!bitmap->getGlyph(glyph, c))
				continue;

			destination.m_x = pen.x;
			destination.m_y = pen.y + glyph->bounds.m_y;
			destination.m_width = glyph->bounds.m_width;
			destination.m_height = glyph->bounds.m_height;

			spriteBatcher.Submit(texture, destination, glyph->bitmapSource, colour);

			pen.x += (glyph->advance >> 6);
		}
	}
}