#pragma once

#include "alvere/math/vector/vector_2.hpp"

namespace alvere
{
	struct Glyph
	{
		Vector2 origin;
		Vector2 min;
		Vector2 max;
		Vector2 bearing;
		Vector2 size;
		float advance;
	};
}