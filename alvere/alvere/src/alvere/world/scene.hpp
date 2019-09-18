#pragma once

#include <typeindex>
#include <unordered_map>
#include <vector>

#include "alvere/world/entity.hpp"
#include "alvere/world/entity_component.hpp"
#include "alvere/world/entity_component_system.hpp"
#include "alvere/world/entity_component_systems/ecs_scene_renderer.hpp"

namespace alvere
{
	class ECSSceneRenderer;

	class Scene
	{
	public:

		Scene();

		Entity * createEntity();

		void destroyEntity(Entity * entity);

		template <typename EntityComponentType>
		EntityComponentType * createEntityComponent(Entity * entity)
		{

		}

		template <typename EntityComponentType>
		void destroyEntityComponent(Entity * entity)
		{

		}

		void setEntityParent(Entity * entity, Entity * parent);

		void addSystem(EntityComponentSystem * system);

		void removeSystem(EntityComponentSystem * system);

		void updateSystems(float timeStep);

	private:

		std::vector<EntityComponentSystem *> m_systems;

		std::vector<Entity> m_entities;

		std::unordered_map<std::type_index, std::pair<unsigned int, EntityComponent *>> m_components;

		ECSSceneRenderer m_sceneRenderer;

	};
}