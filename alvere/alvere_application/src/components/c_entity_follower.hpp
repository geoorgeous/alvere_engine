#pragma once

#include <alvere/world/component/pooled_component.hpp>
#include <alvere/world/entity/entity_handle.hpp>

class C_EntityFollower : public alvere::PooledComponent<C_EntityFollower>
{
public:

	alvere::EntityHandle m_FollowTarget;


	virtual std::string to_string() const
	{
		std::string str = "";

		str += "Has target: " + std::to_string(m_FollowTarget.isValid()) + '\n';

		return str;
	}
};