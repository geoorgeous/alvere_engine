#pragma once

#include <typeindex>
#include <unordered_map>

#include "alvere/utils/uuid.hpp"
#include "alvere/world/entity_component_map.hpp"

namespace alvere
{
	class EntityComponentSystem
	{
	public:

		virtual void updateSystems(EntityComponentMap & entityComponentMap, float timeStep) = 0;
	};
}