#pragma once

#include "alvere/world/entity_component.hpp"
#include "alvere/world/transform.hpp"

namespace alvere
{
	struct ECTransform : public EntityComponent
	{
		Transform transform;
	};
}