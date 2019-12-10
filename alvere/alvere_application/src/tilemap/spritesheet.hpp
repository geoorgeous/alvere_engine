#pragma once

#include <alvere\graphics\texture.hpp>

struct Spritesheet
{
	std::unique_ptr<alvere::Texture> m_texture;
	alvere::Vector2i m_uvTileSize;

	alvere::RectI GetSourceRect(alvere::Vector2i spritesheetCoord) const
	{
		alvere::Vector2i position = spritesheetCoord * m_uvTileSize;
		return alvere::RectI(position[0], position[1], m_uvTileSize[0], m_uvTileSize[1]);
	}
};