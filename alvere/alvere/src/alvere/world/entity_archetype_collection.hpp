#pragma once

#include <unordered_map>
#include <memory>

#include "alvere/world/entity_archetype.hpp"
#include "alvere/world/entity_component_collection.hpp"

namespace alvere
{
	using EntityArchetypeHashCode = std::size_t;

	class EntityArchetypeCollection
	{
	public:

		EntityArchetypeCollection();

		template <typename EntityComponentType>
		void addCollectionType(EntityComponentCollection<EntityComponentType> newCollection)
		{
			m_componentCollections[typeid(EntityComponentType)] = std::make_unique<EntityComponentCollection<EntityComponentType>>(newCollection);

			m_archetypeHashCode *= 31 + std::hash<std::type_index>(typeid(EntityComponentType));
		}

		inline EntityArchetypeHashCode getHashCode() const
		{
			return m_archetypeHashCode;
		}

	private:

		EntityArchetypeHashCode m_archetypeHashCode;

		std::unordered_map<std::type_index, std::unique_ptr<EntityComponentCollectionBase>> m_componentCollections;
	};

	struct EntityArchetypeCollectionHasher
	{
		std::size_t operator()(const EntityArchetypeCollection & entityArchetypeCollection) const
		{
			return entityArchetypeCollection.getHashCode();
		}
	};
}