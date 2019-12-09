#pragma once

#include <vector>

#include <alvere/world/component/pooled_component.hpp>

#include "tile.hpp"

struct C_Tilemap : public alvere::PooledComponent<C_Tilemap>
{
	alvere::Vector2i m_size;
	alvere::Vector2 m_tileSize;

	std::unique_ptr<TileInstance[]> m_map;
	std::vector<Tile> m_tiles;



	C_Tilemap();

	C_Tilemap(alvere::Vector2i size, alvere::Vector2 tileSize = { 1, 1 });


	void SetTile(alvere::Vector2i position, Tile * tile);
	void SetTile_s(alvere::Vector2i position, Tile * tile);

	//These values can be negative
	void Resize(int left, int right, int top, int bottom);

	void UpdateAllTiles();
	void UpdateTiles(alvere::RectI area);
	void UpdateTile(alvere::Vector2i position);

	TileDirection GetUnmatchingSurroundings(alvere::Vector2i position, bool collides) const;

	bool TileCollides_s(alvere::Vector2i position) const;

	alvere::Vector2i WorldToTilemap(alvere::Vector2 worldPosition) const;


	void DemoFill()
	{
		for (std::size_t y = 0; y < m_size[1]; ++y)
		{
			for (std::size_t x = 0; x < m_size[0]; ++x)
			{
				bool isEmpty = (x >= 3 && x < m_size[0] - 3 && y >= 3 && y < m_size[1] - 3);
				m_map[x + y * m_size[0]] = TileInstance{ isEmpty ? &m_tiles[0] : &m_tiles[1] };
			}
		}

		UpdateAllTiles();
	}
};