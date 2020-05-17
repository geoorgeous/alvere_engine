#pragma once

#include <alvere/math/vectors.hpp>
#include <alvere/world/component/pooled_component.hpp>

struct ColliderInstance
{
	alvere::Rect m_LocalBounds;
};

class C_Collider : public alvere::PooledComponent<C_Collider>
{
public:

	std::vector<ColliderInstance> m_ColliderInstances;


	void AddInstance(const ColliderInstance & instance)
	{
		m_ColliderInstances.emplace_back(instance);
	}
};