#include "s_movement.hpp"

S_Movement::S_Movement(float horizontalSpeed, float acceleration)
	: m_HorizontalSpeed(horizontalSpeed)
	, m_Acceleration(acceleration)
{
}

void S_Movement::Update(float deltaTime, C_Movement & movement, C_Velocity & velocity)
{
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