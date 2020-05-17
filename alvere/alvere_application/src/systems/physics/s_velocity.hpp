#pragma once

#include <alvere/world/system/query_updated_system.hpp>
#include <alvere/world/component/components/c_transform.hpp>

#include "components/physics/c_velocity.hpp"

class S_Velocity : public alvere::QueryUpdatedSystem<alvere::C_Transform, C_Velocity>
{
public:

	void Update(float deltaTime, alvere::C_Transform & transform, C_Velocity & velocity)
	{
		transform->move( velocity.m_Velocity * deltaTime );
	}
};
