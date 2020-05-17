#pragma once

#include "alvere/world/component/pooled_component.hpp"

namespace alvere
{
	class C_Direction : public PooledComponent<C_Direction>
	{
	public:
		float m_X;
		float m_Y;
	};
}