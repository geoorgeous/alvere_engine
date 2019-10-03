#pragma once

#include "alvere/world/component/pooled_component.hpp"

struct C_Transform : public PooledComponent<C_Transform>
{
	float m_X;
	float m_Y;
};