#include <string>
#include <vector>

#include <alvere/graphics/texture.hpp>
#include <alvere/utils/assets.hpp>

#include "components/tilemap/c_tilemap.hpp"
#include "editor/imgui_editor.hpp"
#include "editor/editor_world.hpp"
#include "editor/io/world_importer.hpp"
#include "editor/io/serialization_utils.hpp"
#include "editor/windows/tile_window.hpp"

using namespace serialization;

WorldImporter::WorldImporter(ImGuiEditor & editor, alvere::Window & window)
	: m_editor(editor)
	, m_window(window)
{
}

std::unique_ptr<EditorWorld> WorldImporter::operator()(const std::string & filepath)
{
	std::unique_ptr<EditorWorld> world = EditorWorld::New(filepath, m_window);
	std::fstream worldFile(filepath, std::ios_base::in | std::ios::binary);

	ImportTilemap(worldFile, *world->m_tilemap);

	worldFile.close();

	return std::move(world);
}

bool WorldImporter::ImportTilemap(std::fstream & file, C_Tilemap & tilemap)
{
	//Version always comes first so we can tell if this file is compatable
	unsigned int version;
	Read(file, version);

	if (version < C_Tilemap::OLDEST_LOADABLE_SAVE_VERSION
	 || version > C_Tilemap::SAVE_VERSION)
	{
		return false;
	}

	alvere::Vector2i mapSize;
	Read(file, mapSize);

	size_t numTiles;
	Read(file, numTiles);

	std::vector<Tile *> tiles(numTiles);

	TileWindow & tileWindow = *m_editor.GetEditorWindow<TileWindow>();
	for (int i = 0; i < numTiles; ++i)
	{
		Tile tile;

		std::string texturePath = ReadString(file);
		tile.m_spritesheet.m_texture = alvere::AssetManager::getStatic<alvere::Texture>(texturePath);

		Read(file, tile.m_spritesheet.m_tileSize);
		Read(file, tile.m_collides);

		tiles[i] = &tileWindow.GetOrAddTile(tile);
	}

	tilemap = C_Tilemap(mapSize);
	TileInstance * map = tilemap.m_map.get();

	for (int i = 0, ic = mapSize[0] * mapSize[1]; i < ic; ++i)
	{
		TileInstance & tileInstance = map[i];

		int id;
		Read(file, id);

		//Offset id by one as we saved 0 to mean nullptr
		tileInstance.m_tile = id <= 0 || id > tiles.size()
			? nullptr
			: tiles[id - 1];

		Read(file, tileInstance.m_spritesheetCoordinate);
	}

	return true;
}