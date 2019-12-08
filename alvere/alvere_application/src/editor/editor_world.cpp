#include <memory>

#include <alvere\world\system\systems\camera_system.hpp>
#include <alvere\world\system\systems\sprite_renderer_system.hpp>
#include <alvere\world\scene\scene_system.hpp>
#include <alvere/world/component/components/c_camera.hpp>
#include <alvere/world/component/components/c_transform.hpp>
#include <alvere/application/window.hpp>
#include <tilemap\tilemap_renderer_system.hpp>
#include <scenes\platformer_scene.hpp>

#include "editor_world.hpp"

using namespace alvere;

std::unique_ptr<EditorWorld> EditorWorld::New(const std::string & name, const alvere::Window & window)
{
	std::unique_ptr<EditorWorld> editorWorld = std::make_unique<EditorWorld>();
	editorWorld->m_name = name;
	World & world = editorWorld->m_world;

	float worldUnitsOnX = 32;

	float halfWorldUnitsOnX = 0.5f * worldUnitsOnX;
	float screenRatio = (float) window.getHeight() / (float) window.getWidth();

	EntityHandle cameraEntity = world.SpawnEntity<C_Transform, C_Camera>();
	editorWorld->m_camera = &world.GetComponent<C_Camera>(cameraEntity);
	editorWorld->m_camera->setOrthographic(-halfWorldUnitsOnX, halfWorldUnitsOnX, halfWorldUnitsOnX * screenRatio, -halfWorldUnitsOnX * screenRatio, -1.0f, 1.0f);


	alvere::EntityHandle map = world.SpawnEntity<C_Tilemap>();

	Spritesheet airSpritesheet = { alvere::Texture::New("res/img/tiles/air.png"), { 1, 1 } };
	Spritesheet wallSpritesheet = { alvere::Texture::New("res/img/tiles/ground.png"), { 24, 24 } };

	editorWorld->m_tilemap = &world.GetComponent<C_Tilemap>(map);
	*editorWorld->m_tilemap = C_Tilemap({ 7, 7 });
	editorWorld->m_tilemap->m_tiles.push_back(Tile{ false, std::move(airSpritesheet) });
	editorWorld->m_tilemap->m_tiles.push_back(Tile{ true, std::move(wallSpritesheet) });

	editorWorld->m_tilemap->DemoFill();

	SceneSystem * sceneSystem = world.AddSystem<SceneSystem>(world);
	world.AddSystem<CameraSystem>();
	world.AddSystem<TilemapRendererSystem>(*editorWorld->m_camera);
	world.AddSystem<SpriteRendererSystem>(*editorWorld->m_camera);

	return std::move(editorWorld);
}