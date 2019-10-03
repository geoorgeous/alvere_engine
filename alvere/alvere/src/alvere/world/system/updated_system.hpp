#pragma once

#include "alvere/world/world.hpp"
#include "alvere/world/system/system.hpp"

class UpdatedSystem : public System
{
public:

	virtual void Update( World& world, float deltaTime ) = 0;
};