#pragma once

#include "alvere/scene/transform.hpp"
#include "alvere/world/entity_component.hpp"

namespace alvere
{
	struct ECTransform : public EntityComponent
	{
		Transform transform;
	};
}