#pragma once

#include "alvere/world/component/pooled_component.hpp"

class Direction : public PooledComponent<Direction>
{
public:
	float m_X;
	float m_Y;
};