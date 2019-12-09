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
}

void C_Tilemap::UpdateAllTiles()
{
	for (std::size_t y = 0; y < m_size[1]; ++y)
	{
		for (std::size_t x = 0; x < m_size[0]; ++x)
		{
			UpdateTile(x, y);
		}
	}
}

void C_Tilemap::UpdateTile(std::size_t x, std::size_t y)
{
	Tile * tile = m_map[x + y * m_size[0]].m_tile;
	if (tile == nullptr)
	{
		return;
	}

	bool collides = tile->m_collides;
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

	m_map[x + y * m_size[0]].m_spritesheetCoordinate = coordinate;
}

TileDirection C_Tilemap::GetUnmatchingSurroundings(std::size_t x, std::size_t y, bool collides) const
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

bool C_Tilemap::TileCollides_s(std::size_t x, std::size_t y) const
{
	if (x < 0 || y < 0 || x >= m_size[0] || y >= m_size[1])
	{
		return false;
	}

	Tile * tile = m_map[x + y * m_size[0]].m_tile;
	if (tile == nullptr)
	{
		return true;
	}

	return tile == nullptr ? false : tile->m_collides;
}