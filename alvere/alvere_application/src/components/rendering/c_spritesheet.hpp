#pragma once

#include <alvere/math/vectors.hpp>
#include <alvere/utils/shapes.hpp>
#include <alvere/world/component/pooled_component.hpp>

class C_Spritesheet : public alvere::PooledComponent<C_Spritesheet>
{
public:

	alvere::RectI m_SourceRect;
	alvere::Vector2i m_Offset;


	virtual std::string to_string() const
	{
		std::string str = "";

		str += "x: " + std::to_string(m_Offset[0]) + '\n';
		str += "y: " + std::to_string(m_Offset[1]) + '\n';

		return str;
	}
};