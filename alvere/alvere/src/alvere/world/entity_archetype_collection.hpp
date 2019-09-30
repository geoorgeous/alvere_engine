#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>

#include "alvere/world/entity_component_collection.hpp"

namespace alvere
{
	class Entity;

	using EntityArchetypeHashCode = std::size_t;

	class EntityArchetypeCollection
	{
	public:

		friend class Scene;

		EntityArchetypeCollection();

		~EntityArchetypeCollection();

		void addCollectionType(std::type_index entityComponentTypeIndex, EntityComponentCollectionBase * collection)
		{
			m_componentCollections[entityComponentTypeIndex] = collection;

			m_archetypeHashCode ^= std::hash<std::type_index>()(entityComponentTypeIndex);
		}

		inline EntityArchetypeHashCode getHashCode() const
		{
			return m_archetypeHashCode;
		}

		unsigned int allocate();

		void removeEntity(Entity & entity);

		void transferEntity(Entity & entity, EntityArchetypeCollection & destination);

		unsigned int getEntityCount() const;

	private:

		EntityArchetypeHashCode m_archetypeHashCode;

		std::unordered_map<std::type_index, EntityComponentCollectionBase *> m_componentCollections;
	};
}