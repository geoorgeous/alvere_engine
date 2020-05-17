#include "s_player_input.hpp"

#include <alvere/application/window.hpp>

S_PlayerInput::S_PlayerInput(const alvere::Window & window)
	: m_Window(window)
{
}

void S_PlayerInput::Update(float deltaTime, const C_Player & player, C_Movement & movement)
{
	movement.m_Jump = m_Window.getKey(alvere::Key::Space).isDown
				   || m_Window.getKey(alvere::Key::W).isDown;

	movement.m_Horizontal = 0.0f;
	if (m_Window.getKey(alvere::Key::A).isDown
	 || m_Window.getKey(alvere::Key::Left).isDown)
	{
		movement.m_Horizontal += -1.0f;
	}
	if (m_Window.getKey(alvere::Key::D).isDown
	 || m_Window.getKey(alvere::Key::Right).isDown)
	{
		movement.m_Horizontal += 1.0f;
	}
}