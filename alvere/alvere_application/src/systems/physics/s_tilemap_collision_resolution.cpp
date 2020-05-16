#include <alvere/world/archetype/archetype.hpp>
#include <alvere/world/archetype/archetype_provider_iterator.hpp>

#include "s_tilemap_collision_resolution.hpp"
#include <algorithm>

void S_TilemapCollisionResolution::Update(float deltaTime, alvere::C_Transform & transform, C_Velocity & velocity, C_TilemapCollision & tilemapCollision)
{
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

			ResolveCollision(tilemap, transform, velocity);
		}
	}
}

void S_TilemapCollisionResolution::ResolveCollision(const C_Tilemap & tilemap, alvere::C_Transform & transform, C_Velocity & velocity)
{
	alvere::Vector2 transformPosition = transform->getPosition();

	//Store positions of collided tiles
	std::vector<alvere::Vector2i> collidedTiles;

	//TODO: Add the collider component and read the bounds off of that
	alvere::Vector2 colliderLocalCenter(0.5f, 0.5f);
	alvere::Vector2 colliderLocalLowerBound(-0.5f, -0.5f);
	alvere::Vector2 colliderLocalUpperBound(0.5f, 0.5f);

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
				  alvere::Vector2 bCenter = alvere::Vector2(a) + tileHalfSize;
				  return (colliderCenter - aCenter).magnitudeSq() < (colliderCenter - bCenter).magnitudeSq();
			  });

	//We update the position of the entity after calculating the resolution vector for every tile as
	//the earlier tiles we check may change how other tiles want to push the entity out.
	//TODO: Play around with a 'totalResolutionVector' which adds up all the resolutions and applies the 
	//		transform & velocity in one go at the end. The collider center can be updated as we go still.
	for (alvere::Vector2i & tilePosition : collidedTiles)
	{
		alvere::Vector2 resolutionVector = CalculateResolutionVectorFromTile(tilemap, colliderCenter, tilePosition);

		colliderCenter += resolutionVector;
		transform->move(resolutionVector);

		//Cancel any velocity if it is pushing the entity into the tile we are trying to correct them out of
		if ((resolutionVector.x > 0.0f && velocity.m_Velocity.x < 0.0f)
		 || (resolutionVector.x < 0.0f && velocity.m_Velocity.x > 0.0f))
		{
			velocity.m_Velocity.x = 0.0f;
		}

		if ((resolutionVector.y > 0.0f && velocity.m_Velocity.y < 0.0f)
		 || (resolutionVector.y < 0.0f && velocity.m_Velocity.y > 0.0f))
		{
			velocity.m_Velocity.y = 0.0f;
		}
	}
}

alvere::Vector2 S_TilemapCollisionResolution::CalculateResolutionVectorFromTile(const C_Tilemap & tilemap, alvere::Vector2 & colliderCenter, alvere::Vector2i tilePosition)
{
	//TODO: Add the collider component and read the bounds off of that
	alvere::Vector2 colliderSize(1.0f, 1.0f);

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