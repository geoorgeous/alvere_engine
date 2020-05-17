#include "s_jump.hpp"

S_Jump::S_Jump(float strength, float duration)
	: m_Strength(strength)
	, m_Duration(duration)
	, m_DurationRemaining(-1)
{
}

void S_Jump::Update(float deltaTime, C_TilemapCollision & tilemapCollision, C_Movement & movement, C_Velocity & velocity)
{
	if (movement.m_Jump == false)
	{
		m_DurationRemaining = -1;
		return;
	}

	if (tilemapCollision.m_OnGround)
	{
		m_DurationRemaining = m_Duration;
	}
	
	if (m_DurationRemaining > 0.0f)
	{
		m_DurationRemaining -= deltaTime;
		velocity.m_Velocity.y = m_Strength;
	}
}