#include "tilemap/spritesheet.hpp"

alvere::RectI Spritesheet::GetSourceRect(alvere::Vector2i spritesheetCoord) const
{
	alvere::Vector2i position = spritesheetCoord * m_tileSize;
	return alvere::RectI(position[0], position[1], m_tileSize[0], m_tileSize[1]);
}

bool Spritesheet::operator==(const Spritesheet & rhs)
{
	return m_texture == rhs.m_texture
		&& m_tileSize == rhs.m_tileSize;
}
