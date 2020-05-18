#pragma once

#include <alvere/world/component/pooled_component.hpp>

class C_TilemapCollision : public alvere::PooledComponent<C_TilemapCollision>
{
public:

	bool m_OnGround;

	virtual std::string to_string() const
	{
		std::string str = "";

		str += "On ground: " + std::to_string(m_OnGround) + '\n';

		return str;
	}
};