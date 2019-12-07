#include <alvere/alvere.hpp>
#include <alvere/assets.hpp>
#include <alvere/graphics/camera.hpp>
#include <alvere/graphics/mesh.hpp>
#include <alvere/graphics/renderer.hpp>
#include <alvere/graphics/sprite_batcher.hpp>
#include <alvere/graphics/texture.hpp>
#include <alvere/math/constants.hpp>
#include <alvere/math/matrix/transformations.hpp>
#include <alvere/utils/file_reader.hpp>
#include <alvere/utils/exceptions.hpp>
#include <alvere/world/world.hpp>
#include <alvere/world/ecs_testing.hpp>
#include <alvere/world/system/systems/sprite_renderer_system.hpp>
#include <alvere/world/scene/scene.hpp>
#include <alvere\world\scene\scene_system.hpp>
#include <alvere\world\system\systems\camera_system.hpp>
#include <alvere\world\system\systems\destroy_system.hpp>
#include <alvere/world/component/components/c_camera.hpp>
#include <alvere/world/component/components/c_transform.hpp>
#include <alvere/input/key_button.hpp>

#include "tile_drawer.hpp"
#include "world_cell.hpp"
#include "world_cell_area.hpp"
#include "world_generation.hpp"
#include "scenes/testing_scene.hpp"
#include "Scenes/platformer_scene.hpp"
#include "imgui/imgui.h"
#include "editor/imgui_editor.hpp"
#include "tilemap/tilemap_renderer_system.h"

/* todo
 *
 * camera component
 * mover/controller component+systems?
 * rich text formatting
 * command console work
 * command console argument parsing
 */

using namespace alvere;

struct AlvereApplication : public Application
{
	Scene m_Scene;

	World m_world;
	Camera * m_sceneCamera;
	Camera m_uiCamera;

	input::KeyButton m_toggleEditor;
	bool m_editorEnabled;
	ImGuiEditor m_editor;

	AlvereApplication()
		: Application(Window::Properties("Platformer", 1024, 768))
		, m_toggleEditor(alvere::Key::I)
		, m_editorEnabled(false)
		, m_editor(*m_window)
	{
		RunTests();


		float screenRatio = (float) m_window->getWidth() / (float) m_window->getHeight();

		EntityHandle cameraEntity = m_world.SpawnEntity<C_Transform, C_Camera>();
		m_sceneCamera = &m_world.GetComponent<C_Camera>( cameraEntity );
		m_sceneCamera->SetOrthographic(-12 * screenRatio, 12 * screenRatio, 12, -12, -1.0f, 1.0f);
		m_uiCamera.SetOrthographic(0, 800, 800, 0, -1.0f, 1.0f);

		SceneSystem * sceneSystem = m_world.AddSystem<SceneSystem>(m_world);
		m_world.AddSystem<TilemapRendererSystem>(*m_sceneCamera);
		m_world.AddSystem<SpriteRendererSystem>(*m_sceneCamera);
		m_world.AddSystem<DestroySystem>();

		PlatformerScene platformerScene(m_world);
		Scene & platformer = sceneSystem->LoadScene(platformerScene);
	}

	void update(float deltaTime) override
	{
		m_toggleEditor.Update(*m_window);
		if (m_toggleEditor.IsPressed())
		{
			m_editorEnabled = !m_editorEnabled;
		}

		if (m_editorEnabled)
		{
			m_editor.Update(deltaTime);
			return;
		}

		Vector3 velocity;
		Vector3 rotation;

		const float moveSpeed = 4.0f;
		const float turnSpeed = 2.0f;

		if (m_window->getKey(Key::W)) velocity += Camera::up * moveSpeed;
		if (m_window->getKey(Key::S)) velocity -= Camera::up * moveSpeed;
		if (m_window->getKey(Key::D)) velocity += Camera::right * moveSpeed;
		if (m_window->getKey(Key::A)) velocity -= Camera::right * moveSpeed;

		velocity *= deltaTime;
		m_sceneCamera->Move(velocity * m_sceneCamera->GetRotation());
		
		if (m_window->getKey(Key::E)) rotation += Camera::forward * turnSpeed;
		if (m_window->getKey(Key::Q)) rotation -= Camera::forward * turnSpeed;

		m_sceneCamera->Rotate(Quaternion::fromEulerAngles(rotation * deltaTime));


		m_world.Update(deltaTime);
	}

	void render() override
	{
		if (m_editorEnabled)
		{
			m_editor.Render();
		}
		else
		{
			m_world.Render();
		}
	}
};

ALV_NEW_APP_FUNC(AlvereApplication)