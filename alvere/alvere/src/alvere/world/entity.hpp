#pragma once

#include "alvere/world/entity_component.hpp"
#include "alvere/world/entity_components/ec_transform.hpp"
#include "alvere/utils/uuid.hpp"

#include <typeindex>
#include <unordered_map>

namespace alvere
{
	class Entity
	{
	public:

		Entity(UUID guid);

		inline ECTransform & transform()
		{
			return *m_transform;
		}

		inline const ECTransform & transform() const
		{
			return *m_transform;
		}

		template <class EntityComponentType>
		void addComponent()
		{
			static_assert(std::is_base_of<EntityComponent, EntityComponentType>::value);

			auto it = m_components.find(typeid(EntityComponent));

			if (it != m_components.end())
			{
				return;
			}

			return m_components[EntityComponentType];
		}

		template <class EntityComponentType>
		const EntityComponent * getComponent()
		{
			static_assert(std::is_base_of<EntityComponent, EntityComponentType>::value);

			auto it = m_components.find(typeid(EntityComponent));

			if (it == m_components.end())
			{
				return nullptr;
			}

			return it->second;
		}

		inline bool operator==(const Entity & rhs) const
		{
			return m_uuid == rhs.m_uuid;
		}

	private:

		UUID m_uuid;

		ECTransform * m_transform;

		std::unordered_map<std::type_index, EntityComponent *> m_components;
	};
}