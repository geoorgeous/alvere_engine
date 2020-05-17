#pragma once

#include <string>

#include <alvere/debug/logging.hpp>
#include <alvere/math/vectors.hpp>
#include <alvere/world/system/query_updated_system.hpp>

#include "components/physics/c_velocity.hpp"
#include "components/physics/c_gravity.hpp"
#include "components/physics/c_friction.hpp"

class S_Friction : public alvere::QueryUpdatedSystem<C_Friction, C_Velocity, C_Gravity>
{
	alvere::Vector2 m_Friction;

public:

	S_Friction(const alvere::Vector2 & friction)
		: m_Friction(friction)
	{
	}

	void Update(float deltaTime, C_Friction & friction, C_Velocity & velocity, C_Gravity & gravity)
	{
		for (int i = 0; i < 2; ++i)
		{
			if (std::abs(velocity.m_Velocity[i]) < m_Friction[i] * deltaTime)
			{
				velocity.m_Velocity[i] = 0.0f;
			}
			else if (velocity.m_Velocity[i] < 0)
			{
				velocity.m_Velocity[i] += m_Friction[i] * deltaTime;
			}
			else
			{
				velocity.m_Velocity[i] -= m_Friction[i] * deltaTime;
			}
		}
	}
};
