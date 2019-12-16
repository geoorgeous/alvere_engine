#include "alvere/graphics/sprite_batcher.hpp"

#include <algorithm>
#include <stdint.h>

#include "alvere/debug/exceptions.hpp"

namespace alvere
{
	void SpriteBatcher::begin(const alvere::Matrix4& transformationMatrix, SortMode sortMode)
	{
		AlvAssert(!m_HasBegun, "SpriteBatcher.Begin() cannot be called before the current SpriteBatcher batch has ended.");

		m_HasBegun = true;
		m_TransformationMatrix = &transformationMatrix;
		m_SortMode = sortMode;
	}

	void SpriteBatcher::end()
	{
		SortDrawCommands();

		for (const DrawSpriteCommand& command : m_DrawCommands)
		{
			processDrawCommandData(command);
		}

		if (m_SpriteCount > 0)
		{
			flush();
		}

		m_DrawCommands.clear();

		m_HasBegun = false;
		m_TransformationMatrix = nullptr;
	}

	void SpriteBatcher::submit(const Texture * texture, alvere::Vector2 position, alvere::Vector4 tint)
	{
		submit(texture, Rect{ position.x, position.y, (float)texture->getDimensions().x, (float)texture->getDimensions().y }, tint);
	}

	void SpriteBatcher::submit(const Texture * texture, Rect destination, alvere::Vector4 tint)
	{
		submit(texture, destination, RectI{ 0, 0, texture->getDimensions().x, texture->getDimensions().y }, tint);
	}

	void SpriteBatcher::submit(const Texture * texture, Rect destination, RectI source, alvere::Vector4 tint)
	{
		m_DrawCommands.push_back({ texture, destination, source, false, 0.0f, tint });
	}

	void SpriteBatcher::submit(const Font::Face::Bitmap & fontFaceBitmap, const std::string & text, Vector2 position, Vector4 colour)
	{
		const Texture * texture = fontFaceBitmap.getTexture();

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
				pen.y -= fontFaceBitmap.getFontFaceHeight();
				continue;
			}
			else if (c == '\t')
			{
				pen.x = position.x + fontFaceBitmap.getFontFaceMaxAdvance() * 4;
			}

			if ((glyph = fontFaceBitmap.getGlyph(c)) == nullptr)
				continue;

			destination.m_x = pen.x + glyph->bairings[0];
			destination.m_y = pen.y - (glyph->size[1] - glyph->bairings[1]);
			destination.m_width = glyph->size[0];
			destination.m_height = glyph->size[1];

			submit(texture, destination, glyph->bitmapSource, colour);

			pen.x += glyph->advance;
		}
	}

	void SpriteBatcher::submit(const Sprite & sprite)
	{
		submit(&sprite.getTexture(), sprite.bounds(), sprite.textureSource(), sprite.colour());
	}

	SpriteBatcher::DrawSpriteCommand& SpriteBatcher::DrawSpriteCommand::operator=(const DrawSpriteCommand& rhs)
	{
		texture = rhs.texture;
		destination = rhs.destination;
		source = rhs.source;
		sortLayer = rhs.sortLayer;
		tint = rhs.tint;
		return *this;
	}

	void SpriteBatcher::SortDrawCommands()
	{
		switch (m_SortMode)
		{
		case SortMode::Texture:
		{
			std::sort(m_DrawCommands.begin(), m_DrawCommands.end(), [](const DrawSpriteCommand& lhs, const DrawSpriteCommand& rhs)
			{
				return (intptr_t)lhs.texture < (intptr_t)rhs.texture;
			});
			break;
		}

		case SortMode::BackToFront:
		{
			break;
		}

		case SortMode::FrontToBack:
		{
			break;
		}
		}
	}
}