#pragma once

#include <typeindex>
#include <unordered_map>

namespace alvere
{
	class EntityComponentSystem
	{
	public:

		virtual void update(std::unordered_map<std::type_index, std::pair<unsigned int, EntityComponent *>> & components, float timeStep) = 0;
	};
}