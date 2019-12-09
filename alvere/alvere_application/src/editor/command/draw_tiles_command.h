#pragma once

#include <memory>

#include <alvere/utils/shapes.hpp>

#include "editor/command/command.hpp"
#include "tilemap/tile.hpp"
#include "tilemap/c_tilemap.hpp"

class DrawTilesCommand : public Command
{
	C_Tilemap & m_tilemap;
	alvere::RectI m_area;
	Tile * m_tile;

	std::unique_ptr<TileInstance[]> m_oldTiles;

public:

	DrawTilesCommand(C_Tilemap & tilemap, alvere::RectI area, Tile * tile)
		: m_tilemap(tilemap)
		, m_area(alvere::RectI::overlap(area, tilemap.GetBounds()))
		, m_tile(tile)
		, m_oldTiles(std::make_unique<TileInstance[]>(area.m_width * area.m_height))
	{
	}

	void Execute() override
	{
		for (int y = 0; y < m_area.m_height; ++y)
		{
			for (int x = 0; x < m_area.m_width; ++x)
			{
				int localIndex = x + y * m_area.m_width;
				int tilemapIndex = (m_area.m_x + x) + (m_area.m_y + y) * m_tilemap.m_size[0];

				m_oldTiles[localIndex] = m_tilemap.m_map[tilemapIndex];
			}
		}

		m_tilemap.SetTiles(m_area, m_tile);
	}

	void Undo() override
	{
		for (int y = 0; y < m_area.m_height; ++y)
		{
			for (int x = 0; x < m_area.m_width; ++x)
			{
				int localIndex = x + y * m_area.m_width;
				int tilemapIndex = (m_area.m_x + x) + (m_area.m_y + y) * m_tilemap.m_size[0];

				m_tilemap.m_map[tilemapIndex] = m_oldTiles[localIndex];
			}
		}

		m_tilemap.UpdateTiles(alvere::RectI::pad(m_area, { 1, 1 }));
	}

	std::string GetDescription() override
	{
		return "Draw tile";
	}
};