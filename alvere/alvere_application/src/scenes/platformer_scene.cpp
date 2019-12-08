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
	static alvere::Asset<alvere::Texture> textureAsset = alvere::Texture::New("res/img/test.png");
	sprite.m_sprite = alvere::Sprite(*textureAsset.get());

	transform->setPosition(alvere::Vector3{ 1.5, 1.5, 0 });
	transform->setScale(alvere::Vector3{ 4, 4, 1 });

	scene->AddEntity(player);
}

void PlatformerScene::SpawnMap(std::unique_ptr<alvere::Scene> & scene)
{
	alvere::EntityHandle map = m_World.SpawnEntity<C_Tilemap>();

	TileStore* tileStore = new TileStore();

	Spritesheet * airSpritesheet = new Spritesheet(alvere::Texture::New("res/img/tiles/air.png"), alvere::Vector2i{ 1, 1 });
	alvere::Asset<Tile> airTile = std::make_unique<Tile>(Tile{ false, std::unique_ptr<Spritesheet>(airSpritesheet) });

	Spritesheet * wallSpritesheet = new Spritesheet(alvere::Texture::New("res/img/tiles/ground.png"), alvere::Vector2i{ 24, 24 });
	alvere::Asset<Tile> wallTile = std::make_unique<Tile>(Tile{ true, std::unique_ptr<Spritesheet>(wallSpritesheet) });

	tileStore->m_tiles.emplace_back(std::move(airTile));
	tileStore->m_tiles.emplace_back(std::move(wallTile));

	auto & tilemap = m_World.GetComponent<C_Tilemap>(map);
	tilemap = C_Tilemap(*tileStore, { 25, 20 });

	scene->AddEntity(map);
}