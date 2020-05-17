#pragma once

#include <alvere/world/system/query_updated_system.hpp>

#include "components/c_direction.hpp"
#include "components/physics/c_movement.hpp"

class S_DirectionFromMovement : public alvere::QueryUpdatedSystem<C_Direction, const C_Movement>
{
public:

	void Update(float deltaTime, C_Direction & direction, const C_Movement & movement)
	{
		if (movement.m_Horizontal < 0.0f)
		{
			direction.m_Direction = Direction::Left;
		}
		else if (movement.m_Horizontal > 0.0f)
		{
			direction.m_Direction = Direction::Right;
		}
	}
};