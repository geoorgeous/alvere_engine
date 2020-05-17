#pragma once

#include <alvere/math/vectors.hpp>
#include <alvere/world/system/query_updated_system.hpp>

#include "components/physics/c_tilemap_collision.hpp"
#include "components/physics/c_movement.hpp"
#include "components/physics/c_velocity.hpp"

class S_Jump : public alvere::QueryUpdatedSystem<const C_TilemapCollision, const C_Movement, C_Velocity>
{
	const float m_Strength;
	const float m_Duration;

	float m_DurationRemaining;

public:

	S_Jump(float strength, float duration);

	void Update(float deltaTime, const C_TilemapCollision & tilemapCollision, const C_Movement & movement, C_Velocity & velocity);
};