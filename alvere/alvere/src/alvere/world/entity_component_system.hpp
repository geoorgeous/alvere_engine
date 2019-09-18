#pragma once

#include <typeindex>
#include <unordered_map>

#include "alvere/utils/uuid.hpp"

namespace alvere
{
	class EntityComponentSystem
	{
	public:

		virtual void updateSystems(std::unordered_map<std::type_index, std::pair<unsigned int, EntityComponent *>> & components, float timeStep) = 0;
	};
}