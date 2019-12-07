#pragma once

#include <alvere\world\scene\scene.hpp>
#include <alvere\world\world.hpp>
#include <alvere\graphics\camera.hpp>
#include <alvere\input\key_button.hpp>

#include "game_state.hpp"

namespace alvere
{
	class Window;
}

class GameplayState : public GameState
{
	alvere::Window & m_window;

	alvere::Scene m_scene;

	alvere::World m_world;
	alvere::Camera * m_sceneCamera;
	alvere::Camera m_uiCamera;

	alvere::input::KeyButton m_toggleEditor;

public:

	GameplayState(alvere::Window & window);

	GameState * Update(float deltaTime) override;
	void Render() override;
};