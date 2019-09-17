#pragma once

#include "alvere/graphics/camera.hpp"
#include "alvere/world/entity_component.hpp"

namespace alvere
{
	struct ECCamera : public EntityComponent
	{
		Camera camera;
	};
}