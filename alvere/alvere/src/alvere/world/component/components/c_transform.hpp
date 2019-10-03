#pragma once

#include "alvere/world/application/transform.hpp"
#include "alvere/world/component/pooled_component.hpp"

namespace alvere
{
	struct C_Transform : public PooledComponent<C_Transform>
	{
		Transform m_transform;

		inline Transform * operator->()
		{
			return &m_transform;
		}
	};
}