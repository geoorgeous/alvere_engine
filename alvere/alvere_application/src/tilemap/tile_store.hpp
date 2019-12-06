#pragma once

#include <vector>
#include <memory>

#include <alvere/assets.hpp>

#include "tile.hpp"

struct TileStore
{
	std::vector<alvere::Asset<Tile>> m_tiles;

	Tile * GetTile(int index) const
	{
		return m_tiles[index].get();
	}
};