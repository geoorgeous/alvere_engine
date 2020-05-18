#pragma once

#include <alvere/world/component/pooled_component.hpp>

class C_Movement : public alvere::PooledComponent<C_Movement>
{
public:

	//This is all very early prototyping. I may just hard code
	//the inputs directly to the player velocity.
	float m_Horizontal;
	bool m_Jump;

	virtual std::string to_string() const
	{
		std::string str = "";

		str += "Horizontal: " + std::to_string(m_Horizontal) + '\n';
		str += "Jump: " + std::to_string(m_Jump) + '\n';

		return str;
	}
};