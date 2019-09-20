#pragma once

#include <typeindex>
#include <unordered_map>
#include <vector>

#include "alvere/world/entity.hpp"
#include "alvere/world/entity_component.hpp"
#include "alvere/world/entity_component_map.hpp"
#include "alvere/world/entity_component_system.hpp"
#include "alvere/world/entity_component_systems/ecs_scene_renderer.hpp"

namespace alvere
{
	class EntityHandle;

	class Scene
	{
	public:

		friend class EntityHandle;

		Scene();

		EntityHandle createEntity();

		void destroyEntity(EntityHandle & entity);

		template <typename EntityComponentType>
		EntityComponentType * createEntityComponent(EntityHandle & entity)
		{
			if (!entity.isValid())
				return;

			ComponentCollection<EntityComponentType> * collection = m_entityComponentMap.getOrCreate<EntityComponentType>();

			return &collection->newEntityComponent(entity);
		}

		template <typename EntityComponentType>
		void destroyEntityComponent(EntityHandle & entity)
		{

		}

		void setEntityParent(EntityHandle & entity, EntityHandle & parent);

		void orphanEntity(EntityHandle & entity);

		void addSystem(EntityComponentSystem * system);

		void removeSystem(EntityComponentSystem * system);

		void updateSystems(float timeStep);

	private:

		std::vector<EntityComponentSystem *> m_systems;

		std::vector<Entity> m_entities;

		EntityComponentMap m_entityComponentMap;

		ECSSceneRenderer m_sceneRenderer;

	};

	
}