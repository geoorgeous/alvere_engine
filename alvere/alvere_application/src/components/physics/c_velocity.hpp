#pragma once

#include <alvere/math/vectors.hpp>
#include <alvere/world/component/pooled_component.hpp>

class C_Velocity : public alvere::PooledComponent<C_Velocity>
{
public:

	alvere::Vector2 m_Velocity;
};