#pragma once

#include "alvere/math/vectors.hpp"

namespace alvere::render_commands
{
	void setClearColour(const alvere::Vector4& colour);

	void clear();

	void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
}