#pragma once

#include <alvere/math/vectors.hpp>
#include <alvere/world/world.hpp>
#include <alvere/world/system/query_updated_system.hpp>
#include <alvere/world/component/components/c_transform.hpp>

#include "components/c_entity_follower.hpp"

class S_EntityFollower : public alvere::QueryUpdatedSystem<alvere::C_Transform, const C_EntityFollower>
{
	alvere::World & m_World;

public:

	S_EntityFollower(alvere::World & world)
		: m_World(world)
	{
	}

	void Update(float deltaTime, alvere::C_Transform & transform, const C_EntityFollower & follower)
	{
		if (follower.m_FollowTarget.isValid() == false)
		{
			return;
		}

		alvere::C_Transform & followingTransform = m_World.GetComponent<alvere::C_Transform>(follower.m_FollowTarget);
		transform->setPosition(followingTransform->getPosition());
	}
};