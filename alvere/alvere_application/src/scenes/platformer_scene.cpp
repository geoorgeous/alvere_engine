#include <alvere/graphics/texture.hpp>
#include <alvere\world\component\components\c_transform.hpp>
#include <alvere\world\component\components\c_sprite.hpp>
#include <alvere/world/component/components/c_destroy.hpp>

#include "platformer_scene.hpp"
#include "entity_definitions/def_player.hpp"

#include "tilemap/c_tilemap.hpp"

std::unique_ptr<alvere::Scene> PlatformerScene::LoadScene()
{
	std::unique_ptr<alvere::Scene> scene = std::make_unique<alvere::Scene>();

	SpawnMap(scene);

	SpawnFromDefinition<Def_Player>(*scene);
	
	return std::move(scene);
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