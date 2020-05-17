#pragma once

#include <alvere/world/component/pooled_component.hpp>
#include <alvere/world/entity/entity_handle.hpp>

class C_EntityFollower : public alvere::PooledComponent<C_EntityFollower>
{
public:

	alvere::EntityHandle m_FollowTarget;
};