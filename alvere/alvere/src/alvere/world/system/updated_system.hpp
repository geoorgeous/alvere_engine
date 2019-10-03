#pragma once

#include "alvere/world/world.hpp"
#include "alvere/world/system/system.hpp"

namespace alvere
{
	class UpdatedSystem : public System
	{
	public:

		virtual void Update(World & world, float deltaTime) = 0;
	};
}