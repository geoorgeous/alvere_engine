#pragma once

#include <alvere/math/vectors.hpp>
#include <alvere/world/system/query_updated_system.hpp>

#include "components/physics/c_movement.hpp"
#include "components/physics/c_velocity.hpp"

class S_Movement : public alvere::QueryUpdatedSystem<const C_Movement, C_Velocity>
{
	const float m_HorizontalSpeed;
	const float m_Acceleration;

public:

	S_Movement(float horizontalSpeed, float acceleration);

	void Update(float deltaTime, const C_Movement & movement, C_Velocity & velocity);
};