#pragma once

#include <alvere/assets.hpp>
#include <alvere/math/vectors.hpp>

#include "spritesheet.hpp"

struct Tile
{
	bool m_collides;
	Spritesheet m_spritesheet;
};

struct TileInstance
{
	Tile * m_tile;
	alvere::Vector2i m_spritesheetCoordinate;
};

struct TileDirection
{
	TileDirection(bool up, bool up_right, bool right, bool down_right, bool down, bool down_left, bool left, bool up_left)
		: UP(up)
		, UP_RIGHT(up_right)
		, RIGHT(right)
		, DOWN_RIGHT(down_right)
		, DOWN(down)
		, DOWN_LEFT(down_left)
		, LEFT(left)
		, UP_LEFT(up_left)
	{
	}

	//All directions to take up one bit of space ideally
	bool UP : 1;
	bool UP_RIGHT : 1;
	bool RIGHT : 1;
	bool DOWN_RIGHT : 1;
	bool DOWN : 1;
	bool DOWN_LEFT : 1;
	bool LEFT : 1;
	bool UP_LEFT : 1;
};