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
#include <alvere/world/world.hpp>
#include <alvere/world/ecs_testing.hpp>
#include <alvere/utils/exceptions.hpp>
#include <alvere/world/system/systems/sprite_renderer_system.hpp>
#include <alvere/world/scene/scene.hpp>
#include <alvere\world\scene\scene_system.hpp>
#include <alvere\world\system\systems\destroy_system.hpp>
#include <alvere/input/key_button.hpp>

#include "tile_drawer.hpp"
#include "world_cell.hpp"
#include "world_cell_area.hpp"
#include "world_generation.hpp"
#include "scenes/testing_scene.hpp"
#include "Scenes/platformer_scene.hpp"
#include "imgui/imgui.h"
#include "editor/imgui_editor.hpp"

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
	Camera m_sceneCamera;
	Camera m_uiCamera;
	Asset<SpriteBatcher> m_spriteBatcher;

	TileDrawer m_tileDrawer;
	WorldCellArea * m_worldCellArea;

	KeyButton m_toggleEditor;
	bool m_editorEnabled;
	ImGuiEditor m_editor;

	AlvereApplication()
		: Application(Window::Properties("Platformer", 1024, 768))
		, m_tileDrawer("res/img/tilesheet.png")
		, m_toggleEditor(alvere::Key::I)
		, m_editorEnabled(false)
		, m_editor(*m_window)
	{
		RunTests();


		float screenRatio = m_window->getWidth() / m_window->getHeight();

		m_sceneCamera.SetPosition(0, 0, 0);
		m_sceneCamera.SetOrthographic(0, 20 * screenRatio, 20, 0, -1.0f, 1.0f);
		m_uiCamera.SetOrthographic(0, 800, 800, 0, -1.0f, 1.0f);


		SceneSystem * sceneSystem = m_world.AddSystem<SceneSystem>(m_world);
		m_world.AddSystem<SpriteRendererSystem>(m_sceneCamera);
		m_world.AddSystem<DestroySystem>();

		PlatformerScene platformerScene(m_world);
		Scene & platformer = sceneSystem->LoadScene(platformerScene);


		m_spriteBatcher = SpriteBatcher::New();
		world_generation::Generate(m_worldCellArea, 0);
	}

	~AlvereApplication()
	{
		delete m_worldCellArea;
	}

	void update(float deltaTime) override
	{
		Vector3 velocity;
		Vector3 rotation;

		const float moveSpeed = 4.0f;
		const float turnSpeed = 2.0f;
		const float mouseDamper = 0.1f;

		if (m_window->getKey(Key::W)) velocity += Camera::forward * moveSpeed;
		if (m_window->getKey(Key::S)) velocity -= Camera::forward * moveSpeed;
		if (m_window->getKey(Key::D)) velocity += Camera::right * moveSpeed;
		if (m_window->getKey(Key::A)) velocity -= Camera::right * moveSpeed;
		if (m_window->getKey(Key::Space)) velocity += Camera::up * moveSpeed;
		if (m_window->getKey(Key::LeftShift)) velocity -= Camera::up * moveSpeed;

		velocity *= deltaTime;
		m_sceneCamera.Move(velocity * m_sceneCamera.GetRotation());

		if (m_window->getMouse().getButton(MouseButton::Left))
		{
			m_window->disableCursor();

			rotation -= Camera::up * m_window->getMouse().delta.x * mouseDamper;
			rotation -= Camera::right * m_window->getMouse().delta.y * mouseDamper;
		}
		else
		{
			m_window->enableCursor();

			if (m_window->getKey(Key::Left)) rotation += Camera::up * turnSpeed;
			if (m_window->getKey(Key::Right)) rotation -= Camera::up * turnSpeed;
			if (m_window->getKey(Key::Up)) rotation += Camera::right * turnSpeed;
			if (m_window->getKey(Key::Down)) rotation -= Camera::right * turnSpeed;
		}
		
		if (m_window->getKey(Key::E)) rotation += Camera::forward * turnSpeed;
		if (m_window->getKey(Key::Q)) rotation -= Camera::forward * turnSpeed;

		m_sceneCamera.Rotate(Quaternion::fromEulerAngles(rotation * deltaTime));

		m_toggleEditor.Update(*m_window);
		if (m_toggleEditor.IsPressed())
		{
			m_editorEnabled = !m_editorEnabled;
		}

		m_world.Update(deltaTime);
	}

	void render() override
	{
		m_spriteBatcher->begin(m_sceneCamera.GetProjectionViewMatrix());
		 
		for (unsigned int x = 0; x < m_worldCellArea->GetWidth(); x++)
		{
			for (unsigned int y = 0; y < m_worldCellArea->GetHeight(); y++)
			{
				WorldCell& cell = m_worldCellArea->At(x, y);
				m_tileDrawer.DrawTile(*m_spriteBatcher, cell.m_x, cell.m_y, cell.m_Type);
			}
		}

		m_spriteBatcher->end();


		m_world.Render();

		if (m_editorEnabled)
		{
			m_editor.Render();
		}
	}
};

ALV_NEW_APP_FUNC(AlvereApplication)