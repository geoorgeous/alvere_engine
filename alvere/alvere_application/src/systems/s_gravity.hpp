#pragma once

#include <alvere/math/vectors.hpp>
#include <alvere/world/system/query_updated_system.hpp>
#include <alvere/world/component/components/c_transform.hpp>

#include "components/c_gravity.hpp"

class S_Gravity : public alvere::QueryUpdatedSystem<alvere::C_Transform, C_Gravity>
{
	alvere::Vector3 m_Gravity;

public:

	S_Gravity(const alvere::Vector2 & gravity)
		: m_Gravity( gravity.x, gravity.y, 0.0f )
	{
	}

	void Update(float deltaTime, alvere::C_Transform & transform, C_Gravity & gravity)
	{
		transform.m_transform.move( m_Gravity * deltaTime );
	}
};
