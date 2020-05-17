#pragma once

#include <memory>
#include <unordered_map>

#include <alvere/utils/shapes.hpp>

#include "components/tilemap/c_tilemap.hpp"
#include "editor/command/command.hpp"
#include "tilemap/tile.hpp"

class DrawTilesCommand : public Command
{
	EditorWorld & m_world;
	TileInstance m_tile;

	alvere::RectI m_boundingArea;
	std::unordered_map<int, TileInstance> m_tiles;

public:

	DrawTilesCommand(EditorWorld & world, TileInstance tile)
		: m_world(world)
		, m_tile(tile)
	{
	}

	void AddDrawArea(alvere::RectI area)
	{
		C_Tilemap & tilemap = *m_world.m_tilemap;

		area = alvere::RectI::overlap(area, tilemap.GetBounds());

		alvere::RectI updateArea = alvere::RectI::pad(area, { 1, 1 });
		m_boundingArea = alvere::RectI::encapsulate(m_boundingArea, updateArea);

		for (int y = 0; y < area.m_height; ++y)
		{
			for (int x = 0; x < area.m_width; ++x)
			{
				int index = (area.m_x + x) + (area.m_y + y) * tilemap.m_size[0];

				auto iter = m_tiles.find(index);

				if (iter != m_tiles.end())
				{
					continue;
				}

				m_tiles.emplace(index, tilemap.m_map[index]);
				tilemap.m_map[index] = m_tile;
			}
		}

		tilemap.UpdateTiles(updateArea);
		m_world.m_dirty = true;
	}

	void Undo() override
	{
		C_Tilemap & tilemap = *m_world.m_tilemap;

		for (auto & pair : m_tiles)
		{
			tilemap.m_map[pair.first] = pair.second;
		}

		tilemap.UpdateTiles(m_boundingArea);
		m_world.m_dirty = true;
	}

	void Redo() override
	{
		C_Tilemap & tilemap = *m_world.m_tilemap;

		for (auto & pair : m_tiles)
		{
			tilemap.m_map[pair.first] = m_tile;
		}

		tilemap.UpdateTiles(m_boundingArea);
		m_world.m_dirty = true;
	}

	std::string GetDescription() override
	{
		return "Draw tile";
	}
};