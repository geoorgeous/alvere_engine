#include "c_tilemap.hpp"


C_Tilemap::C_Tilemap()
	: m_size({ 0, 0 })
	, m_tileSize({ 0, 0 })
{
}

C_Tilemap::C_Tilemap(alvere::Vector2i size, alvere::Vector2 tileSize)
	: m_size(size)
	, m_tileSize(tileSize)
	, m_map(std::make_unique<TileInstance[]>(size[0] * size[1]))
{
}

//These values can be negative
void C_Tilemap::Resize(int left, int right, int top, int bottom)
{
	alvere::Vector2i newSize = m_size + alvere::Vector2i{ left + right, top + bottom };
	newSize = alvere::Vector2i::max({ 0, 0 }, newSize);

	std::unique_ptr<TileInstance[]> newMap = std::make_unique<TileInstance[]>(newSize[0] * newSize[1]);

	alvere::Vector2i currentPos;
	alvere::Vector2i oldOffset{ -left, -bottom };

	for (currentPos[1] = 0; currentPos[1] < newSize[1]; ++currentPos[1])
	{
		for (currentPos[0] = 0; currentPos[0] < newSize[0]; ++currentPos[0])
		{
			alvere::Vector2i oldPos = currentPos + oldOffset;

			bool inBounds = oldPos[0] >= 0 && oldPos[0] < m_size[0] && oldPos[1] >= 0 && oldPos[1] < m_size[1];

			newMap[currentPos[0] + currentPos[1] * newSize[0]] = inBounds
				? m_map[oldPos[0] + oldPos[1] * m_size[0]]
				: TileInstance{ nullptr };
		}
	}

	m_size = newSize;
	m_map = std::move(newMap);
	UpdateTiles(GetBounds());
}

void C_Tilemap::UpdateTiles(alvere::RectI area)
{
	//Ensure the given area is within the tilemap bounds
	area = alvere::RectI::overlap(area, {0, 0, m_size[0], m_size[1]});

	for (int y = 0; y < area.m_height; ++y)
	{
		for (int x = 0; x < area.m_width; ++x)
		{
			UpdateTile({ area.m_x + x, area.m_y + y });
		}
	}
}

void C_Tilemap::UpdateTile(alvere::Vector2i position)
{
	TileInstance & tileInstance = m_map[position[0] + position[1] * m_size[0]];

	if (tileInstance.m_tile == nullptr)
	{
		return;
	}

	bool collides = tileInstance.m_tile->m_collides;
	TileDirection surr = GetUnmatchingSurroundings(position, collides);

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

	tileInstance.m_spritesheetCoordinate = coordinate;
}

void C_Tilemap::SetTiles(alvere::RectI area, Tile * tile)
{
	//Ensure the given area is within the tilemap bounds
	area = alvere::RectI::overlap(area, GetBounds());

	for (int y = 0; y < area.m_height; ++y)
	{
		for (int x = 0; x < area.m_width; ++x)
		{
			SetTile_Unsafe({ area.m_x + x, area.m_y + y }, tile);
		}
	}

	//Bulk update the all tiles effected and their neighbours
	UpdateTiles(alvere::RectI::pad(area, { 1, 1 }));
}

void C_Tilemap::SetTile(alvere::Vector2i position, Tile * tile)
{
	if (position[0] < 0 || position[0] >= m_size[0] || position[1] < 0 || position[1] >= m_size[1])
	{
		return;
	}

	SetTile_Unsafe(position, tile);
	UpdateTiles({ position[0] - 1, position[1] - 1, 3, 3 });
}

void C_Tilemap::SetTile_Unsafe(alvere::Vector2i position, Tile * tile)
{
	TileInstance & tileInstance = m_map[position[0] + position[1] * m_size[0]];
	tileInstance.m_tile = tile;
}

alvere::Vector2i C_Tilemap::WorldToTilemap(alvere::Vector2 worldPosition) const
{
	alvere::Vector2 tileSpace = worldPosition / m_tileSize;
	return { (int) std::floor(tileSpace.x), (int) std::floor(tileSpace.y) };
}

alvere::Vector2 C_Tilemap::TilemapToWorld(alvere::Vector2i tilemapPosition) const
{
	alvere::Vector2 centerPosition = alvere::Vector2(tilemapPosition) + alvere::Vector2{ 0.5f, 0.5f };
	return centerPosition * m_tileSize;
}

alvere::Vector2 C_Tilemap::WorldToLocal(alvere::Vector2 worldPosition) const
{
	return worldPosition / m_tileSize;
}

alvere::Vector2 C_Tilemap::LocalToWorld(alvere::Vector2 worldPosition) const
{
	return worldPosition * m_tileSize;
}

TileDirection C_Tilemap::GetUnmatchingSurroundings(alvere::Vector2i position, bool collides) const
{
	return TileDirection(
		TileCollides_s(position + alvere::Vector2i{  0,  1 }) != collides,
		TileCollides_s(position + alvere::Vector2i{  1,  1 }) != collides,
		TileCollides_s(position + alvere::Vector2i{  1,  0 }) != collides,
		TileCollides_s(position + alvere::Vector2i{  1, -1 }) != collides,
		TileCollides_s(position + alvere::Vector2i{  0, -1 }) != collides,
		TileCollides_s(position + alvere::Vector2i{ -1, -1 }) != collides,
		TileCollides_s(position + alvere::Vector2i{ -1,  0 }) != collides,
		TileCollides_s(position + alvere::Vector2i{ -1,  1 }) != collides
	);
}

bool C_Tilemap::TileCollides_s(alvere::Vector2i position) const
{
	if (position[0] < 0 || position[1] < 0 || position[0] >= m_size[0] || position[1] >= m_size[1])
	{
		return false;
	}

	Tile * tile = m_map[position[0] + position[1] * m_size[0]].m_tile;
	if (tile == nullptr)
	{
		return true;
	}

	return tile == nullptr ? false : tile->m_collides;
}

void C_Tilemap::DemoFill()
{
	SetTiles(GetBounds(), &m_tiles[1]);
	SetTiles({ 3, 3, m_size[0] - 7, m_size[1] - 7 }, &m_tiles[0]);
}