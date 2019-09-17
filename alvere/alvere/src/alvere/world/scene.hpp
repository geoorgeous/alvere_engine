#pragma once

#include "alvere/world/entity.hpp"
#include "alvere/world/entity_component.hpp"
#include "alvere/world/entity_component_system.hpp"

#include <typeindex>
#include <unordered_map>
#include <vector>

namespace alvere
{
	class Scene
	{
	public:

		void update(float timeStep);

		Entity * createEntity();

		void destroyEntity(Entity * entity);

	private:

		std::vector<EntityComponentSystem> m_systems;

		std::vector<Entity> m_entities;

		std::unordered_map<std::type_index, std::pair<unsigned int, EntityComponent *>> m_components;

	};
}