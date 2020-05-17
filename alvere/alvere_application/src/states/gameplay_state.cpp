#include <alvere\application\window.hpp>
#include <alvere/world/ecs_testing.hpp>
#include <alvere/world/world.hpp>
#include <alvere/world/scene/scene.hpp>
#include <alvere\world\scene\scene_system.hpp>

#include <alvere/world/system/systems/s_sprite_renderer.hpp>
#include <alvere\world\system\systems\s_destroy.hpp>
#include <alvere\world\system\systems\s_camera.hpp>

#include <alvere/world/component/components/c_camera.hpp>
#include <alvere/world/component/components/c_transform.hpp>

#include "gameplay_state.hpp"
#include "states/editor_state.hpp"
#include "scenes/platformer_scene.hpp"
#include "entity_definitions/def_camera.hpp"

#include "components/c_entity_follower.hpp"
#include "components/c_player.hpp"

#include "systems/tilemap/s_tilemap_renderer.hpp"
#include "systems/physics/s_tilemap_collision_resolution.hpp"
#include "systems/physics/s_gravity.hpp"
#include "systems/physics/s_velocity.hpp"
#include "systems/physics/s_friction.hpp"
#include "systems/input/s_player_input.hpp"
#include "systems/rendering/s_spritesheet.hpp"
#include "systems/s_entity_follower.hpp"
#include "systems/s_movement.hpp"
#include "systems/s_jump.hpp"

GameplayState::GameplayState(alvere::Window & window)
	: m_window(window), m_toggleEditor(window, alvere::Key::I), m_halfWorldUnitsOnX(32 * 0.5f)
{
	alvere::RunTests();

	alvere::EntityHandle cameraEntity;
	{ //Camera instance
		float screenRatio = window.getRenderingContext().getAspectRatio();

		cameraEntity = Def_Camera().SpawnInstance(m_world);
		m_sceneCamera = &m_world.GetComponent<alvere::C_Camera>(cameraEntity);
		m_sceneCamera->setOrthographic(-m_halfWorldUnitsOnX, m_halfWorldUnitsOnX, m_halfWorldUnitsOnX * screenRatio, -m_halfWorldUnitsOnX * screenRatio, -1.0f, 1.0f);
		m_uiCamera.setOrthographic(0, 800, 800, 0, -1.0f, 1.0f);
	}

	alvere::SceneSystem * sceneSystem = m_world.AddSystem<alvere::SceneSystem>(m_world);

	m_world.AddSystem<alvere::S_Destroy>();
	m_world.AddSystem<S_PlayerInput>(m_window);
	m_world.AddSystem<S_Movement>(15.0f, 10.0f);
	m_world.AddSystem<S_Jump>(20.0f, 0.2f);
	m_world.AddSystem<S_Gravity>(alvere::Vector2(0.0f, -70.0f));
	m_world.AddSystem<S_Friction>(alvere::Vector2(100.0f, 0.0f));
	m_world.AddSystem<S_Velocity>();
	m_world.AddSystem<S_TilemapCollisionResolution>(m_world);
	m_world.AddSystem<S_EntityFollower>(m_world);
	m_world.AddSystem<alvere::S_Camera>();

	m_world.AddSystem<S_Spritesheet>();
	m_world.AddSystem<S_TilemapRenderer>(*m_sceneCamera);
	m_world.AddSystem<alvere::S_SpriteRenderer>(*m_sceneCamera);

	PlatformerScene platformerScene(m_world);
	alvere::Scene & platformer = sceneSystem->LoadScene(platformerScene);

	{ //Camera follow setup
		//TODO: I need some way to query for the player here. Some way of getting out the player entity handle given a PlayerTag component or something
		alvere::Archetype::Query playerQuery;
		playerQuery.Include<C_Player>();
		std::vector<std::reference_wrapper<alvere::Archetype>> archetypes;
		m_world.QueryArchetypes(playerQuery, archetypes);

		if (archetypes.empty() == false)
		{
			const std::unordered_set<alvere::EntityHandle, alvere::EntityHandle::Hash> entities = archetypes[0].get().GetEntities();
			for (alvere::EntityHandle entity : entities)
			{
				C_EntityFollower & cameraFollower = m_world.GetComponent<C_EntityFollower>(cameraEntity);
				cameraFollower.m_FollowTarget = entity;
				break;
			}
		}
	}

	m_windowResizeEventHandler.setFunction([ & ](unsigned int width, unsigned int height)
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

	/*
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
	*/

	m_world.Update(deltaTime);

	return nullptr;
}

void GameplayState::Render()
{
	m_world.Render();
}