#include "alvere/world/entity_archetype_collection.hpp"

#include "alvere/world/entity.hpp"

namespace alvere
{
	EntityArchetypeCollection::EntityArchetypeCollection()
		: m_archetypeHashCode(17)
	{ }

	EntityArchetypeCollection::~EntityArchetypeCollection()
	{
		for (auto & pair : m_componentCollections)
			delete pair.second;
	}

	unsigned int EntityArchetypeCollection::allocate()
	{
		for (auto & pair : m_componentCollections)
			pair.second->allocate();

		return m_componentCollections.begin()->second->getCount() - 1;
	}

	unsigned int EntityArchetypeCollection::transferEntity(unsigned int index, EntityArchetypeCollection & destination)
	{
		unsigned int distinationIndex = destination.allocate();

		for (auto & pair : m_componentCollections)
			pair.second->transferComponent(index, destination.m_componentCollections[pair.first], distinationIndex);

		return distinationIndex;
	}

	unsigned int EntityArchetypeCollection::getEntityCount() const
	{
		if (m_componentCollections.size() == 0)
			return 0;

		return m_componentCollections.begin()->second->getCount();
	}
}