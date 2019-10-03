#pragma once

#include "alvere/world/world.hpp"
#include "alvere/world/system/system.hpp"

namespace alvere
{
	class RenderedSystem : public System
	{
	public:

		virtual void Render(World & world) = 0;
	};
}