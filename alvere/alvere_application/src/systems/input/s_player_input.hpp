#pragma once

#include <alvere/math/vectors.hpp>
#include <alvere/world/system/query_updated_system.hpp>

#include "components/c_player.hpp"
#include "components/physics/c_gravity.hpp"
#include "components/physics/c_movement.hpp"

namespace alvere
{
	class Window;
}

class S_PlayerInput : public alvere::QueryUpdatedSystem<const C_Player, C_Movement>
{
	const alvere::Window & m_Window;
	
public:

	S_PlayerInput(const alvere::Window & window);

	void Update(float deltaTime, const C_Player & player, C_Movement & movement);
};
