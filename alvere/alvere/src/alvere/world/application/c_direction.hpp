#pragma once

#include "alvere/world/component/pooled_component.hpp"

namespace alvere
{
	class Direction : public PooledComponent<Direction>
	{
	public:
		float m_X;
		float m_Y;
	};
}