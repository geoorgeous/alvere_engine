#include <memory>
#include <string>

#include <alvere/utils/assets.hpp>
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

	EntityHandle cameraEntity = world.SpawnEntity<C_Transform, C_Camera>();
	editorWorld->m_camera = &world.GetComponent<C_Camera>(cameraEntity);
	editorWorld->m_camera->setOrthographic(-halfWorldUnitsOnX, halfWorldUnitsOnX, halfWorldUnitsOnX * window.getRenderingContext().getAspectRatio(), -halfWorldUnitsOnX * window.getRenderingContext().getAspectRatio(), -1.0f, 1.0f);


	alvere::EntityHandle map = world.SpawnEntity<C_Tilemap>();

	Spritesheet airSpritesheet = { alvere::AssetManager::getStatic<alvere::Texture>("res/img/tiles/air.png"), { 1, 1 } };
	Spritesheet wallSpritesheet = { alvere::AssetManager::getStatic<alvere::Texture>("res/img/tiles/ground.png"), { 24, 24 } };

	editorWorld->m_tilemap = &world.GetComponent<C_Tilemap>(map);
	*editorWorld->m_tilemap = C_Tilemap({ 7, 7 });
	editorWorld->m_tilemap->SetTiles(editorWorld->m_tilemap->GetBounds(), nullptr);

	SceneSystem * sceneSystem = world.AddSystem<SceneSystem>(world);
	world.AddSystem<CameraSystem>();
	world.AddSystem<TilemapRendererSystem>(*editorWorld->m_camera);
	world.AddSystem<SpriteRendererSystem>(*editorWorld->m_camera);

	return std::move(editorWorld);
}