#pragma once

#include "alvere/world/component/pooled_component.hpp"

struct C_Mover : public PooledComponent<C_Mover>
{
	float m_Speed;
};