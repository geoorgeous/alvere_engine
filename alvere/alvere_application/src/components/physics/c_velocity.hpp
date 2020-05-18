#pragma once

#include <alvere/math/vectors.hpp>
#include <alvere/world/component/pooled_component.hpp>

class C_Velocity : public alvere::PooledComponent<C_Velocity>
{
public:

	alvere::Vector2 m_Velocity;


	virtual std::string to_string() const
	{
		std::string str = "";

		str += "x: " + std::to_string(m_Velocity.x) + '\n';
		str += "y: " + std::to_string(m_Velocity.y) + '\n';

		return str;
	}
};