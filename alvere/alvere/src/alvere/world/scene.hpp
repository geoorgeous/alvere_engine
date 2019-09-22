#pragma once

#include <typeindex>
#include <unordered_map>
#include <vector>

#include "alvere/world/entity.hpp"
#include "alvere/world/entity_handle.hpp"
#include "alvere/world/entity_component.hpp"
#include "alvere/world/entity_component_map.hpp"
#include "alvere/world/entity_component_system.hpp"
#include "alvere/world/entity_component_systems/ecs_scene_renderer.hpp"

namespace alvere
{
	class Scene
	{
	public:

		friend class EntityHandle;

		Scene();

		EntityHandle createEntity();

		void destroyEntity(EntityHandle & entityHandle);

		template <typename EntityComponentType>
		EntityComponentType * createEntityComponent(EntityHandle & entityHandle)
		{
			static_assert(std::is_base_of<EntityComponent, EntityComponentType>::value);

			if (!entityHandle.isValid())
				return nullptr;

			ComponentCollection<EntityComponentType> * collection = m_entityComponentMap.getOrCreate<EntityComponentType>();

			return &collection->newEntityComponent(entityHandle);
		}

		template <typename EntityComponentType>
		void destroyEntityComponent(EntityHandle & entityHandle)
		{
			static_assert(std::is_base_of<EntityComponent, EntityComponentType>::value);

			if (!entityHandle.isValid())
				return;

			ComponentCollection<EntityComponentType> * collection = m_entityComponentMap.get<EntityComponentType>();

			if (collection == nullptr)
				return;

			auto iter = collection->components.begin();
			for (; iter != collection->components().end();)
			{
				if (iter->entityHandle == entityHandle)
					iter = collection->components().erase(iter);
				else
					++iter;
			}
		}

		void setEntityParent(EntityHandle & entityHandle, EntityHandle & parentHandle);

		void orphanEntity(EntityHandle & entityHandle);

		void addSystem(EntityComponentSystem * system);

		void removeSystem(EntityComponentSystem * system);

		void updateSystems(float timeStep);

	private:

		std::vector<EntityComponentSystem *> m_systems;

		std::unordered_map<UUID, Entity> m_entities;

		EntityComponentMap m_entityComponentMap;

		ECSSceneRenderer m_sceneRenderer;

	};
}