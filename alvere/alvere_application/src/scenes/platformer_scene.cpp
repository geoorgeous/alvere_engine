#include <alvere/graphics/texture.hpp>
#include <alvere\world\component\components\c_transform.hpp>
#include <alvere\world\component\components\c_sprite.hpp>
#include <alvere/world/component/components/c_destroy.hpp>

#include "platformer_scene.hpp"
#include "tilemap/c_tilemap.hpp"

std::unique_ptr<alvere::Scene> PlatformerScene::LoadScene()
{
	std::unique_ptr<alvere::Scene> scene = std::make_unique<alvere::Scene>();

	SpawnMap(scene);
	SpawnPlayer(scene);
	
	return std::move(scene);
}

void PlatformerScene::SpawnPlayer(std::unique_ptr<alvere::Scene> & scene)
{
	alvere::EntityHandle player = m_World.SpawnEntity<alvere::C_Transform, alvere::C_Sprite>();
	auto & sprite = m_World.GetComponent<alvere::C_Sprite>(player);
	auto & transform = m_World.GetComponent<alvere::C_Transform>(player);

	//Have to make this static for now as the asset going out of scope deletes my texture reference
	static std::unique_ptr<alvere::Texture> textureAsset = alvere::Texture::New("res/img/test.png");

	sprite.m_sprite = alvere::Sprite(*textureAsset.get());

	//transform->setScale(alvere::Vector3{ 4, 4, 1 });

	scene->AddEntity(player);
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