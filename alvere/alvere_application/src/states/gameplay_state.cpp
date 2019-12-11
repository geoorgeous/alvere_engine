#include <alvere\application\window.hpp>
#include <alvere/world/ecs_testing.hpp>
#include <alvere/world/world.hpp>
#include <alvere/world/system/systems/sprite_renderer_system.hpp>
#include <alvere/world/scene/scene.hpp>
#include <alvere\world\scene\scene_system.hpp>
#include <alvere\world\system\systems\camera_system.hpp>
#include <alvere\world\system\systems\destroy_system.hpp>
#include <alvere/world/component/components/c_camera.hpp>
#include <alvere/world/component/components/c_transform.hpp>

#include "gameplay_state.hpp"
#include "scenes/platformer_scene.hpp"
#include "states/editor_state.hpp"
#include "tilemap/tilemap_renderer_system.hpp"

GameplayState::GameplayState(alvere::Window & window)
	: m_window(window), m_toggleEditor(window, alvere::Key::I), m_halfWorldUnitsOnX(32 * 0.5f)

{
	alvere::RunTests();

	//float screenRatio = (float)window.getHeight() / window.getWidth();
	float screenRatio = window.getRenderingContext().getAspectRatio();

	alvere::EntityHandle cameraEntity = m_world.SpawnEntity<alvere::C_Transform, alvere::C_Camera>();
	m_sceneCamera = &m_world.GetComponent<alvere::C_Camera>(cameraEntity);
	m_sceneCamera->setOrthographic(-m_halfWorldUnitsOnX, m_halfWorldUnitsOnX, m_halfWorldUnitsOnX * screenRatio, -m_halfWorldUnitsOnX * screenRatio, -1.0f, 1.0f);
	m_uiCamera.setOrthographic(0, 800, 800, 0, -1.0f, 1.0f);

	alvere::SceneSystem * sceneSystem = m_world.AddSystem<alvere::SceneSystem>(m_world);
	m_world.AddSystem<TilemapRendererSystem>(*m_sceneCamera);
	m_world.AddSystem<alvere::SpriteRendererSystem>(*m_sceneCamera);
	m_world.AddSystem<alvere::DestroySystem>();

	PlatformerScene platformerScene(m_world);
	alvere::Scene & platformer = sceneSystem->LoadScene(platformerScene);

	m_windowResizeEventHandler.setFunction([&](unsigned int width, unsigned int height)
	{
		float screenRatio = window.getRenderingContext().getAspectRatio();
		m_sceneCamera->setOrthographic(-m_halfWorldUnitsOnX, m_halfWorldUnitsOnX, m_halfWorldUnitsOnX * screenRatio, -m_halfWorldUnitsOnX * screenRatio, -1.0f, 1.0f);
	});
	*m_window.getEvent<alvere::WindowResizeEvent>() += m_windowResizeEventHandler;
}

GameplayState::~GameplayState()
{
	*m_window.getEvent<alvere::WindowResizeEvent>() -= m_windowResizeEventHandler;
}

GameState * GameplayState::Update(float deltaTime)
{
	m_toggleEditor.update();
	if (m_toggleEditor.isReleased())
	{
		return new EditorState(m_window);
	}

	{ //TODO: Remove these and maybe put them in a debug component?
		alvere::Vector3 velocity;
		alvere::Vector3 rotation;

		const float moveSpeed = 4.0f;
		const float turnSpeed = 2.0f;

		if (m_window.getKey(alvere::Key::W)) velocity += alvere::Camera::s_up * moveSpeed;
		if (m_window.getKey(alvere::Key::S)) velocity -= alvere::Camera::s_up * moveSpeed;
		if (m_window.getKey(alvere::Key::D)) velocity += alvere::Camera::s_right * moveSpeed;
		if (m_window.getKey(alvere::Key::A)) velocity -= alvere::Camera::s_right * moveSpeed;

		velocity *= deltaTime;
		m_sceneCamera->move(velocity * m_sceneCamera->getRotation());

		if (m_window.getKey(alvere::Key::E)) rotation += alvere::Camera::s_forward * turnSpeed;
		if (m_window.getKey(alvere::Key::Q)) rotation -= alvere::Camera::s_forward * turnSpeed;

		if(m_window.getKey(alvere::Key::F11).justReleased)
		{
			m_window.setFlag(alvere::Window::Flag::FullScreen, !m_window.getFlag(alvere::Window::Flag::FullScreen));
		}

		if(m_window.getKey(alvere::Key::D1).justReleased)
		{
			m_window.setCursor(alvere::CursorType::Arrow);
		}

		m_sceneCamera->rotate(alvere::Quaternion::fromEulerAngles(rotation * deltaTime));
	}

	m_world.Update(deltaTime);

	return nullptr;
}

void GameplayState::Render()
{
	m_world.Render();
}