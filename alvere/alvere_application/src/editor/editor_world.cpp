#include <memory>
#include <string>

#include <alvere/utils/assets.hpp>
#include <alvere\world\system\systems\s_camera.hpp>
#include <alvere\world\system\systems\s_sprite_renderer.hpp>
#include <alvere\world\scene\scene_system.hpp>
#include <alvere/world/component/components/c_camera.hpp>
#include <alvere/world/component/components/c_transform.hpp>
#include <alvere/application/window.hpp>
#include <scenes\platformer_scene.hpp>

#include "editor/editor_world.hpp"
#include "editor/utils/path_utils.hpp"
#include "entity_definitions/def_player.hpp"

#include "systems/tilemap/s_tilemap_renderer.hpp"

using namespace alvere;

std::unique_ptr<EditorWorld> EditorWorld::New(const std::string & filepath, const alvere::Window & window)
{
	std::unique_ptr<EditorWorld> editorWorld = std::make_unique<EditorWorld>();
	editorWorld->m_filepath = filepath;
	World & world = editorWorld->m_world;

	float worldUnitsOnX = 32;
	float halfWorldUnitsOnX = 0.5f * worldUnitsOnX;

	EntityHandle cameraEntity = world.SpawnEntity<C_Transform, C_Camera>();
	editorWorld->m_camera = &world.GetComponent<C_Camera>(cameraEntity);
	editorWorld->m_camera->setOrthographic(-halfWorldUnitsOnX, halfWorldUnitsOnX, halfWorldUnitsOnX * window.getRenderingContext().getAspectRatio(), -halfWorldUnitsOnX * window.getRenderingContext().getAspectRatio(), -1.0f, 1.0f);

	//Def_Player().SpawnInstance(world);

	alvere::EntityHandle map = world.SpawnEntity<C_Tilemap>();

	Spritesheet airSpritesheet = { alvere::AssetManager::getStatic<alvere::Texture>("res/img/tiles/air.png"), { 1, 1 } };
	Spritesheet wallSpritesheet = { alvere::AssetManager::getStatic<alvere::Texture>("res/img/tiles/ground.png"), { 24, 24 } };

	editorWorld->m_tilemap = &world.GetComponent<C_Tilemap>(map);
	*editorWorld->m_tilemap = C_Tilemap({ 7, 7 });
	editorWorld->m_tilemap->SetTiles(editorWorld->m_tilemap->GetBounds(), nullptr);

	SceneSystem * sceneSystem = world.AddSystem<SceneSystem>(world);
	world.AddSystem<S_Camera>();
	world.AddSystem<S_TilemapRenderer>(*editorWorld->m_camera);
	world.AddSystem<S_SpriteRenderer>(*editorWorld->m_camera);

	return std::move(editorWorld);
}

std::string EditorWorld::GetName() const
{
	std::string filename;
	GetFilenameFromPath(m_filepath, filename);
	return filename;
}