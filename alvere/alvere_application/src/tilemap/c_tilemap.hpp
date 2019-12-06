#pragma once

#include <vector>

#include <alvere/world/component/pooled_component.hpp>

#include "tile.hpp"
#include "tile_store.hpp"

struct C_Tilemap : public alvere::PooledComponent<C_Tilemap>
{
	alvere::Vector2i m_size;
	alvere::Vector2 m_tileSize;

	std::vector<TileInstance> m_map;

	TileStore * m_tileStore;

	C_Tilemap()
		: m_tileStore(nullptr)
		, m_size({ 0, 0 })
		, m_tileSize({ 0, 0 })
	{
	}

	C_Tilemap(TileStore & tiles, alvere::Vector2i size, alvere::Vector2 tileSize = { 1, 1 })
		: m_tileStore(&tiles)
		, m_size(size)
		, m_tileSize(tileSize)
		, m_map((std::size_t) size[0] * (std::size_t) size[1])
	{
		for (std::size_t y = 0; y < size[1]; ++y)
		{
			for (std::size_t x = 0; x < size[0]; ++x)
			{
				bool isEmpty = (x >= 3 && x < size[0] - 3 && y >= 3 && y < size[1] - 3);
				m_map[x + y * size[0]] = TileInstance{ isEmpty ? m_tileStore->GetTile(0) : m_tileStore->GetTile(1) };
			}
		}

		for (std::size_t y = 0; y < size[1]; ++y)
		{
			for (std::size_t x = 0; x < size[0]; ++x)
			{
				bool collides = m_map[x + y * m_size[0]].m_tile->m_collides;
				TileDirection surr = GetUnmatchingSurroundings(x, y, collides);

				alvere::Vector2i coordinate;

				if (surr.UP && surr.LEFT)         coordinate = { 0, 2 };
				else if (surr.UP && surr.RIGHT)   coordinate = { 2, 2 };
				else if (surr.DOWN && surr.LEFT)  coordinate = { 0, 0 };
				else if (surr.DOWN && surr.RIGHT) coordinate = { 2, 0 };
				else if (surr.UP)			coordinate = { 1, 2 };
				else if (surr.DOWN)			coordinate = { 1, 0 };
				else if (surr.LEFT)			coordinate = { 0, 1 };
				else if (surr.RIGHT)		coordinate = { 2, 1 };
				else if (surr.UP_LEFT)		coordinate = { 4, 1 };
				else if (surr.UP_RIGHT)		coordinate = { 3, 1 };
				else if (surr.DOWN_LEFT)	coordinate = { 4, 2 };
				else if (surr.DOWN_RIGHT)	coordinate = { 3, 2 };
				else						coordinate = { 1, 1 };

				m_map[x + y * size[0]].m_spritesheetCoordinate = coordinate;
			}
		}
	}

	TileDirection GetUnmatchingSurroundings(std::size_t x, std::size_t y, bool collides) const
	{
		return TileDirection(
			TileCollides_s(x, y + 1) != collides,
			TileCollides_s(x + 1, y + 1) != collides,
			TileCollides_s(x + 1, y) != collides,
			TileCollides_s(x + 1, y - 1) != collides,
			TileCollides_s(x, y - 1) != collides,
			TileCollides_s(x - 1, y - 1) != collides,
			TileCollides_s(x - 1, y) != collides,
			TileCollides_s(x - 1, y + 1) != collides
		);
	}

	bool TileCollides_s(std::size_t x, std::size_t y) const
	{
		return x < 0 || y < 0 || x >= m_size[0] || y >= m_size[1] ? false : m_map[x + y * m_size[0]].m_tile->m_collides;
	}
};