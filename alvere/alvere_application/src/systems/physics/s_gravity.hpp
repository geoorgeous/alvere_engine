#pragma once

#include <alvere/math/vectors.hpp>
#include <alvere/world/system/query_updated_system.hpp>

#include "components/physics/c_velocity.hpp"
#include "components/physics/c_gravity.hpp"

class S_Gravity : public alvere::QueryUpdatedSystem<const C_Gravity, C_Velocity>
{
	alvere::Vector3 m_Gravity;

public:

	S_Gravity(const alvere::Vector2 & gravity)
		: m_Gravity( gravity.x, gravity.y, 0.0f )
	{
	}

	void Update(float deltaTime, const C_Gravity & gravity, C_Velocity & velocity)
	{
		velocity.m_Velocity += m_Gravity * deltaTime;
	}
};
