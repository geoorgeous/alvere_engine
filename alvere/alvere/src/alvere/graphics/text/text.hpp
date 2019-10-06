#pragma once

#include <string>

#include "alvere/graphics/text/font.hpp"
#include "alvere/math/vector/vector_3.hpp"

namespace alvere
{
	struct Text
	{
		std::string m_content;

		Font::Style m_style;

		Vector3 m_colour;
	};
}