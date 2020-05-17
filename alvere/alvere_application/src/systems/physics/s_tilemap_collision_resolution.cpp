#include <algorithm>

#include <alvere/world/archetype/archetype.hpp>
#include <alvere/world/archetype/archetype_provider_iterator.hpp>

#include "s_tilemap_collision_resolution.hpp"

void S_TilemapCollisionResolution::Update(float deltaTime, alvere::C_Transform & transform, C_Velocity & velocity, const C_Collider & collider, C_TilemapCollision & tilemapCollision)
{
	//Reset all physics flags
	tilemapCollision.m_OnGround = false;

	//Find all the tilemaps in the world
	alvere::Archetype::Query query;
	query.Include<C_Tilemap>();

	std::vector<std::reference_wrapper<alvere::Archetype>> matchingArchetypes;
	m_World.QueryArchetypes(query, matchingArchetypes);

	for (alvere::Archetype & archetype : matchingArchetypes)
	{
		alvere::ArchetypeProviderIterator<C_Tilemap> iterator(archetype.GetEntityCount(), archetype.GetProvider<C_Tilemap>());

		for (; iterator; ++iterator)
		{
			auto queryTuple = iterator.GetComponents();
			C_Tilemap & tilemap = std::get<0>(queryTuple);

			ResolveCollision(tilemap, tilemapCollision, collider, transform, velocity);
		}
	}
}

void S_TilemapCollisionResolution::ResolveCollision(const C_Tilemap & tilemap, C_TilemapCollision & tilemapCollision, const C_Collider & collider, alvere::C_Transform & transform, C_Velocity & velocity)
{
	for (const ColliderInstance & colliderInstance : collider.m_ColliderInstances)
	{
		ResolveCollisionWithCollider(tilemap, tilemapCollision, colliderInstance, transform, velocity);
	}
}

void S_TilemapCollisionResolution::ResolveCollisionWithCollider(const C_Tilemap & tilemap, C_TilemapCollision & tilemapCollision, const ColliderInstance & collider, alvere::C_Transform & transform, C_Velocity & velocity)
{
	alvere::Vector2 transformPosition = transform->getPosition();

	//Store positions of collided tiles
	std::vector<alvere::Vector2i> collidedTiles;

	//Grab collider info
	alvere::Vector2 colliderLocalLowerBound = collider.m_LocalBounds.getBottomLeft();
	alvere::Vector2 colliderLocalUpperBound = collider.m_LocalBounds.getTopRight();
	alvere::Vector2 colliderLocalSize = colliderLocalUpperBound - colliderLocalLowerBound;
	alvere::Vector2 colliderLocalCenter = (colliderLocalLowerBound + colliderLocalUpperBound) / 2.0f;

	//Get bounds in tiles
	alvere::Vector2i lower = tilemap.WorldToTilemap(transformPosition + colliderLocalLowerBound);
	alvere::Vector2i upper = tilemap.WorldToTilemap(transformPosition + colliderLocalUpperBound);

	//No need to process collision if the collider isn't within the tilemap bounds
	alvere::RectI localBounds = tilemap.GetBounds();
	if (localBounds.contains(lower) == false
	 && localBounds.contains(upper) == false)
	{
		return;
	}

	//Push all tiles onto vector with their center as world coords
	for (int y = lower[1]; y <= upper[1]; ++y)
	{
		for (int x = lower[0]; x <= upper[0]; ++x)
		{
			alvere::Vector2i queryPosition{ x, y };
			if (tilemap.TileCollides_s(queryPosition))
			{
				collidedTiles.emplace_back(queryPosition);
			}
		}
	}

	//If no tiles collide then ignore next steps
	if (collidedTiles.empty())
	{
		return;
	}

	//Get the center of the entity's collider in local space relative to the tilemap
	alvere::Vector2 colliderCenter = tilemap.WorldToLocal(transformPosition + colliderLocalCenter);

	//Sort the tiles that collide by their distance to the collider center.
	//This is to stop a tile further away resolving collision first and getting the shortest resolution vector wrong.
	alvere::Vector2 tileHalfSize = tilemap.m_tileSize / 2.0f;
	std::sort(collidedTiles.begin(),
			  collidedTiles.end(),
			  [ = ](alvere::Vector2i a, alvere::Vector2i b)
			  {
				  alvere::Vector2 aCenter = alvere::Vector2(a) + tileHalfSize;
				  alvere::Vector2 bCenter = alvere::Vector2(b) + tileHalfSize;
				  return (colliderCenter - aCenter).magnitudeSq() < (colliderCenter - bCenter).magnitudeSq();
			  });

	//We update the position of the entity after calculating the resolution vector for every tile as
	//the earlier tiles we check may change how other tiles want to push the entity out.
	//TODO: Play around with a 'totalResolutionVector' which adds up all the resolutions and applies the 
	//		transform & velocity in one go at the end. The collider center can be updated as we go still.
	for (alvere::Vector2i & tilePosition : collidedTiles)
	{
		alvere::Vector2 resolutionVector = CalculateResolutionVectorFromTile(tilemap, colliderCenter, colliderLocalSize, tilePosition);

		colliderCenter += resolutionVector;
		transform->move(resolutionVector);

		bool hitLeft = resolutionVector.x > 0.0f && velocity.m_Velocity.x < 0.0f;
		bool hitRight = resolutionVector.x < 0.0f && velocity.m_Velocity.x > 0.0f;
		bool hitFloor = resolutionVector.y > 0.0f && velocity.m_Velocity.y < 0.0f;
		bool hitCeiling = resolutionVector.y < 0.0f && velocity.m_Velocity.y > 0.0f;

		//Physics flags
		tilemapCollision.m_OnGround |= hitFloor;

		//Cancel any velocity if it is pushing the entity into the tile we are trying to correct them out of
		if (hitLeft || hitRight)
		{
			velocity.m_Velocity.x = 0.0f;
		}
		if (hitFloor || hitCeiling)
		{
			velocity.m_Velocity.y = 0.0f;
		}
	}
}

alvere::Vector2 S_TilemapCollisionResolution::CalculateResolutionVectorFromTile(const C_Tilemap & tilemap, alvere::Vector2 colliderCenter, alvere::Vector2 colliderSize, alvere::Vector2i tilePosition)
{
	alvere::Vector2 tileHalfSize = tilemap.m_tileSize / 2.0f;
	alvere::Vector2 collisionExtents = colliderSize / 2.0 + tileHalfSize;
	alvere::Vector2 dist = colliderCenter - ( alvere::Vector2(tilePosition) + tileHalfSize );
	alvere::Vector2 collisionDepth = collisionExtents - alvere::Vector2(std::abs(dist.x), std::abs(dist.y));

	if (collisionDepth.x <= 0.0 || collisionDepth.y <= 0.0)
	{
		return alvere::Vector2( 0.0f, 0.0f );
	}

	//Resolution vector is now whichever axis has the least amount of collision depth
	alvere::Vector2 resolutionVector(0.0f, 0.0f);
	if (collisionDepth.x < collisionDepth.y)
	{
		if (dist.x > 0.0)
		{
			resolutionVector.x += collisionDepth.x;
		}
		else
		{
			resolutionVector.x -= collisionDepth.x;
		}
	}
	else
	{
		if (dist.y > 0.0)
		{
			resolutionVector.y += collisionDepth.y;
		}
		else
		{
			resolutionVector.y -= collisionDepth.y;
		}
	}

	return resolutionVector;
}