#include <fstream>

#include <alvere/graphics/texture.hpp>
#include <alvere\world\component\components\c_transform.hpp>

#include "platformer_scene.hpp"
#include "entity_definitions/def_player.hpp"

#include "components/tilemap/c_tilemap.hpp"

std::unique_ptr<alvere::Scene> PlatformerScene::LoadScene()
{
	std::unique_ptr<alvere::Scene> scene = std::make_unique<alvere::Scene>();

	if (LoadMap(scene, "res/maps/default.map") == false)
	{
		SpawnMap(scene);
	}

	alvere::EntityHandle player = SpawnFromDefinition<Def_Player>(*scene);
	alvere::C_Transform & playerTransform = m_World.GetComponent<alvere::C_Transform>(player);
	playerTransform->setPosition({ 4.0f, 4.0f, 0.0f });

	return std::move(scene);
}

bool PlatformerScene::LoadMap(std::unique_ptr<alvere::Scene> & scene, const std::string & filepath)
{
	std::fstream worldFile(filepath, std::ios_base::in | std::ios::binary);

	if (worldFile.is_open() == false)
	{
		return false;
	}

	alvere::EntityHandle tilemapEntity = m_World.SpawnEntity<C_Tilemap>();
	auto & tilemap = m_World.GetComponent<C_Tilemap>(tilemapEntity);
	
	if (tilemap.Load(worldFile) == false)
	{
		m_World.DestroyEntity(tilemapEntity);
		return false;
	}
	
	scene->AddEntity(tilemapEntity);
	return true;
}

void PlatformerScene::SpawnMap(std::unique_ptr<alvere::Scene> & scene)
{
	alvere::EntityHandle map = m_World.SpawnEntity<C_Tilemap>();

	Spritesheet airSpritesheet = { alvere::AssetManager::getStatic<alvere::Texture>("res/img/tiles/air.png"), { 1, 1 } };
	Spritesheet wallSpritesheet = { alvere::AssetManager::getStatic<alvere::Texture>("res/img/tiles/ground.png"), { 24, 24 } };

	auto & tilemap = m_World.GetComponent<C_Tilemap>(map);
	tilemap = C_Tilemap({ 25, 20 });
	tilemap.m_tiles.push_back(Tile{ false, std::move(airSpritesheet) });
	tilemap.m_tiles.push_back(Tile{ true, std::move(wallSpritesheet) });

	tilemap.DemoFill();

	scene->AddEntity(map);
}