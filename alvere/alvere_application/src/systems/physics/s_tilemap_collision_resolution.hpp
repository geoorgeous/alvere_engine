#pragma once

#include <alvere/math/vectors.hpp>
#include <alvere/world/world.hpp>
#include <alvere/world/system/query_updated_system.hpp>
#include <alvere/world/component/components/c_transform.hpp>

#include "tilemap/c_tilemap.hpp"
#include "tilemap/tile.hpp"
#include "components/physics/c_tilemap_collision.hpp"
#include "components/physics/c_velocity.hpp"

class S_TilemapCollisionResolution : public alvere::QueryUpdatedSystem<alvere::C_Transform, C_Velocity, C_TilemapCollision>
{
	typedef alvere::QueryUpdatedSystem<alvere::C_Transform, C_TilemapCollision> BaseType;

	alvere::World & m_World;

public:

	S_TilemapCollisionResolution(alvere::World & world)
		: m_World( world )
	{
	}

	void Update(float deltaTime, alvere::C_Transform & transform, C_Velocity & velocity, C_TilemapCollision & tilemapCollision);

	void ResolveCollision(const C_Tilemap & tilemap, C_TilemapCollision & tilemapCollision, alvere::C_Transform & transform, C_Velocity & velocity);
	alvere::Vector2 CalculateResolutionVectorFromTile(const C_Tilemap & level, alvere::Vector2 & position, alvere::Vector2i tilePosition);

};
