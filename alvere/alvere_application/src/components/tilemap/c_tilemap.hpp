#pragma once

#include <vector>

#include <alvere/world/component/pooled_component.hpp>

#include "tilemap/tile.hpp"

struct C_Tilemap : public alvere::PooledComponent<C_Tilemap>
{
	const static unsigned int SAVE_VERSION = 1u;
	const static unsigned int OLDEST_LOADABLE_SAVE_VERSION = 1u;

	alvere::Vector2i m_size;
	alvere::Vector2 m_tileSize;

	std::unique_ptr<TileInstance[]> m_map;
	std::vector<Tile> m_tiles;



	C_Tilemap();
	C_Tilemap(alvere::Vector2i size, alvere::Vector2 tileSize = { 1, 1 });

	void SetTiles(alvere::RectI area, Tile * tile);
	void SetTile(alvere::Vector2i position, Tile * tile);
	void SetTile_Unsafe(alvere::Vector2i position, Tile * tile);

	void UpdateTiles(alvere::RectI area);
	void UpdateTile(alvere::Vector2i position);

	void Resize(int left, int right, int top, int bottom); //These values can be negative

	alvere::RectI GetBounds() const { return { 0, 0, m_size[0], m_size[1] }; }

	alvere::Vector2i WorldToTilemap(alvere::Vector2 worldPosition) const;
	alvere::Vector2 TilemapToWorld(alvere::Vector2i tilemapPosition) const;

	alvere::Vector2 WorldToLocal(alvere::Vector2 worldPosition) const;
	alvere::Vector2 LocalToWorld(alvere::Vector2 localPosition) const;

	bool Load(std::fstream & file);

	virtual std::string to_string() const;

	//These methods are temporary
	TileDirection GetUnmatchingSurroundings(alvere::Vector2i position, bool collides) const;
	bool TileCollides_s(alvere::Vector2i position) const;
	void DemoFill();
};