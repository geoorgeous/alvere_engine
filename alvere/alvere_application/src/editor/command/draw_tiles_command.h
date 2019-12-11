#pragma once

#include <memory>
#include <unordered_map>

#include <alvere/utils/shapes.hpp>

#include "editor/command/command.hpp"
#include "tilemap/tile.hpp"
#include "tilemap/c_tilemap.hpp"

class DrawTilesCommand : public Command
{
	C_Tilemap & m_tilemap;
	TileInstance m_tile;

	alvere::RectI m_boundingArea;
	std::unordered_map<int, TileInstance> m_tiles;

public:

	DrawTilesCommand(C_Tilemap & tilemap, TileInstance tile)
		: m_tilemap(tilemap)
		, m_tile(tile)
	{
	}

	void AddDrawArea(alvere::RectI area)
	{
		area = alvere::RectI::overlap(area, m_tilemap.GetBounds());

		alvere::RectI updateArea = alvere::RectI::pad(area, { 1, 1 });
		m_boundingArea = alvere::RectI::encapsulate(m_boundingArea, updateArea);

		for (int y = 0; y < area.m_height; ++y)
		{
			for (int x = 0; x < area.m_width; ++x)
			{
				int index = (area.m_x + x) + (area.m_y + y) * m_tilemap.m_size[0];

				auto iter = m_tiles.find(index);

				if (iter != m_tiles.end())
				{
					continue;
				}

				m_tiles.emplace(index, m_tilemap.m_map[index]);
				m_tilemap.m_map[index] = m_tile;
			}
		}

		m_tilemap.UpdateTiles(updateArea);
	}

	void Undo() override
	{
		for (auto & pair : m_tiles)
		{
			m_tilemap.m_map[pair.first] = pair.second;
		}

		m_tilemap.UpdateTiles(m_boundingArea);
	}

	void Redo() override
	{
		for (auto & pair : m_tiles)
		{
			m_tilemap.m_map[pair.first] = m_tile;
		}

		m_tilemap.UpdateTiles(m_boundingArea);
	}

	std::string GetDescription() override
	{
		return "Draw tile";
	}
};