#pragma once

#include <alvere/math/vectors.hpp>
#include <alvere/world/component/pooled_component.hpp>

enum class Direction
{
	Right,
	Left
};

class C_Direction : public alvere::PooledComponent<C_Direction>
{
public:

	Direction m_Direction;


	virtual std::string to_string() const
	{
		std::string str = "";

		std::string dir = m_Direction == Direction::Right ? "Right" : "Left";
		str += "Direction: " + dir + '\n';

		return str;
	}
};