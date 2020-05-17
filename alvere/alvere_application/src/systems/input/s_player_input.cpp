#include "s_player_input.hpp"

#include <alvere/application/window.hpp>

S_PlayerInput::S_PlayerInput(const alvere::Window & window, float horizontalSpeed, float acceleration)
	: m_Window(window)
	, m_HorizontalSpeed(horizontalSpeed)
	, m_Acceleration(acceleration)
{
}

void S_PlayerInput::Update(float deltaTime, C_Player & player, C_Movement & movement, C_Velocity & velocity)
{
	movement.m_Horizontal = 0.0f;
	if (m_Window.getKey(alvere::Key::A).isDown)
	{
		movement.m_Horizontal = -1.0f;
	}
	
	if (m_Window.getKey(alvere::Key::D).isDown)
	{
		movement.m_Horizontal = 1.0f;
	}

	movement.m_Jump = m_Window.getKey(alvere::Key::Space).isDown 
				   || m_Window.getKey(alvere::Key::W).isDown;

	float speed = 0.0f;
	if (movement.m_Horizontal < 0.0f)
	{
		speed = -m_HorizontalSpeed;
	}
	else if (movement.m_Horizontal > 0.0f)
	{
		speed = m_HorizontalSpeed;
	}

	if ((speed > 0.0 && velocity.m_Velocity.x < speed)
	 || (speed < 0.0 && velocity.m_Velocity.x > speed))
	{
		velocity.m_Velocity.x += speed * m_Acceleration * deltaTime;
	}
}