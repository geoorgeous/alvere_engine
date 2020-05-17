#pragma once

#include <alvere/world/component/pooled_component.hpp>

class C_TilemapCollision : public alvere::PooledComponent<C_TilemapCollision>
{
public:

	bool m_OnGround;
};