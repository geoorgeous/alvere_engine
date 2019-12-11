#include <string>
#include <unordered_map>

#include "editor/editor_world.hpp"
#include "editor/io/world_exporter.hpp"
#include "editor/io/serialization_utils.hpp"
#include "tilemap/c_tilemap.hpp"

using namespace serialization;

void WorldExporter::operator()(const std::string & filepath, const EditorWorld & world)
{
	std::fstream worldFile(filepath, std::ios_base::out | std::ios::binary);

	ExportTilemap(worldFile, *world.m_tilemap);

	worldFile.close();
}

void WorldExporter::ExportTilemap(std::fstream & file, const C_Tilemap & tilemap)
{
	int mapSize = tilemap.m_size[0] * tilemap.m_size[1];
	TileInstance * map = tilemap.m_map.get();

	//First get all the unique tiles used in the map
	std::unordered_map<Tile *, unsigned int> uniqueTiles;
	for (int i = 0; i < mapSize; ++i)
	{
		TileInstance & tileInstance = map[i];

		if (tileInstance.m_tile == nullptr)
		{
			//No point serializing null as it has no properties
			continue;
		}

		auto iter = uniqueTiles.find(tileInstance.m_tile);
		if (iter != uniqueTiles.end())
		{
			continue;
		}

		//Reserve id 0 for if the tile is null
		uniqueTiles.emplace(tileInstance.m_tile, uniqueTiles.size() + 1u);
	}

	//Append all the tiles
	Write(file, uniqueTiles.size());
	for (auto & pair : uniqueTiles)
	{
		Tile & tile = *pair.first;

		WriteString(file, tile.m_spritesheet.m_texture.getFilepath());
		Write(file, tile.m_spritesheet.m_tileSize);
		Write(file, tile.m_collides);
	}

	//Append the map itself
	Write(file, tilemap.m_size);
	for (int i = 0; i < mapSize; ++i)
	{
		TileInstance & tileInstance = map[i];

		unsigned int id = tileInstance.m_tile == nullptr
			? 0u
			: uniqueTiles[tileInstance.m_tile];

		Write(file, id);
		Write(file, tileInstance.m_spritesheetCoordinate);
	}
}