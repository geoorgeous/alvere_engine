#include <alvere\world\system\systems\camera_system.hpp>
#include <alvere\world\system\systems\sprite_renderer_system.hpp>
#include <alvere\world\scene\scene_system.hpp>
#include <alvere/world/component/components/c_camera.hpp>
#include <alvere/world/component/components/c_transform.hpp>
#include <alvere/application/window.hpp>
#include <tilemap\tilemap_renderer_system.h>
#include <scenes\platformer_scene.hpp>

#include "editor_world.hpp"

using namespace alvere;

EditorWorld EditorWorld::New(const std::string & name, const alvere::Window & window)
{
	World world;

	float screenRatio = (float) window.getWidth() / (float) window.getHeight();

	EntityHandle cameraEntity = world.SpawnEntity<C_Transform, C_Camera>();
	Camera & camera = world.GetComponent<C_Camera>(cameraEntity);
	float cameraHalfWidth = 19.5;
	camera.SetOrthographic(-cameraHalfWidth * screenRatio, cameraHalfWidth * screenRatio, cameraHalfWidth, -cameraHalfWidth, -1.0f, 1.0f);

	SceneSystem * sceneSystem = world.AddSystem<SceneSystem>(world);
	world.AddSystem<CameraSystem>();
	world.AddSystem<TilemapRendererSystem>(camera);
	world.AddSystem<SpriteRendererSystem>(camera);

	PlatformerScene platformerScene(world);
	Scene & platformer = sceneSystem->LoadScene(platformerScene);

	return { name, std::move(world) };
}