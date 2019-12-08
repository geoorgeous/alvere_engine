#pragma once

#include <alvere\graphics\texture.hpp>

class Spritesheet
{
	alvere::Asset<alvere::Texture> m_texture;
	alvere::Vector2i m_uvTileSize;

public:

	Spritesheet(alvere::Asset<alvere::Texture> texture, alvere::Vector2i tilePixelSize)
		: m_texture(std::move(texture))
		, m_uvTileSize(tilePixelSize)
	{
	}

	alvere::RectI GetSourceRect(alvere::Vector2i spritesheetCoord) const
	{
		alvere::Vector2i position = spritesheetCoord * m_uvTileSize;
		return alvere::RectI(position[0], position[1], m_uvTileSize[0], m_uvTileSize[1]);
	}

	const alvere::Asset<alvere::Texture> & GetTexture() const
	{
		return m_texture;
	}
};