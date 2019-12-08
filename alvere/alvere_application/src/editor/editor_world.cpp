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

	SceneSystem * sceneSystem = world.AddSystem<SceneSystem>(world);
	world.AddSystem<CameraSystem>();
	world.AddSystem<TilemapRendererSystem>(*editorWorld->m_camera);
	world.AddSystem<SpriteRendererSystem>(*editorWorld->m_camera);

	PlatformerScene platformerScene(world);
	Scene & platformer = sceneSystem->LoadScene(platformerScene);

	return std::move(editorWorld);
}