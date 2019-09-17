#include <algorithm>
#include <stdint.h>

#include "alvere/graphics/sprite_batcher.hpp"
#include "alvere/utils/exceptions.hpp"

namespace alvere
{
	void SpriteBatcher::Begin(const alvere::Matrix4& transformationMatrix, SortMode sortMode)
	{
		AlvWasAssert(!m_HasBegun, "SpriteBatcher.Begin() cannot be called before the current SpriteBatcher batch has ended.");

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

	void SpriteBatcher::Submit(const Texture * texture, alvere::Vector2 position, alvere::Vector4 tint)
	{
		Submit(texture, Rect{ position.x, position.y, (float)texture->width(), (float)texture->height() }, tint);
	}

	void SpriteBatcher::Submit(const Texture * texture, Rect destination, alvere::Vector4 tint)
	{
		Submit(texture, destination, RectI{ 0, 0, texture->width(), texture->height() }, tint);
	}

	void SpriteBatcher::Submit(const Texture * texture, Rect destination, RectI source, alvere::Vector4 tint)
	{
		m_DrawCommands.push_back({ texture, destination, source, false, 0.0f, tint });
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