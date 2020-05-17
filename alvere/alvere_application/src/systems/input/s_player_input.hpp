#pragma once

#include <alvere/math/vectors.hpp>
#include <alvere/world/system/query_updated_system.hpp>

#include "components/c_player.hpp"
#include "components/physics/c_gravity.hpp"
#include "components/physics/c_movement.hpp"
#include "components/physics/c_velocity.hpp"

namespace alvere
{
	class Window;
}

class S_PlayerInput : public alvere::QueryUpdatedSystem<C_Player, C_Movement, C_Velocity>
{
	const alvere::Window & m_Window;
	float m_HorizontalSpeed;
	float m_Acceleration;

public:

	S_PlayerInput(const alvere::Window & window, float horizontalSpeed, float acceleration);

	void Update(float deltaTime, C_Player & player, C_Movement & movement, C_Velocity & velocity);
};
