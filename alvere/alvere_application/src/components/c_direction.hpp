#pragma once

#include <alvere/math/vectors.hpp>
#include <alvere/world/component/pooled_component.hpp>

enum class Direction
{
	Left,
	Right
};

class C_Direction : public alvere::PooledComponent<C_Direction>
{
public:

	Direction m_Direction;
};