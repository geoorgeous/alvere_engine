#include "tilemap/tile.hpp"

bool Tile::operator==(const Tile & rhs)
{
	return m_collides == rhs.m_collides
		&& m_spritesheet == rhs.m_spritesheet;
}