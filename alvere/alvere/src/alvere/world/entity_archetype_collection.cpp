#include "alvere/world/entity_archetype_collection.hpp"

#include "alvere/world/entity.hpp"

namespace alvere
{
	EntityArchetypeCollection::EntityArchetypeCollection()
		: m_archetypeHashCode(0)
	{ }

	EntityArchetypeCollection::~EntityArchetypeCollection()
	{
		// todo: make copy constructor that copys collections so we can delete pointers in here
	}

	unsigned int EntityArchetypeCollection::allocate()
	{
		for (auto & pair : m_componentCollections)
			pair.second->allocate();

		return m_componentCollections.begin()->second->getCount() - 1;
	}

	void EntityArchetypeCollection::removeEntity(Entity & entity)
	{
		for (auto & pair : m_componentCollections)
			pair.second->removeAt(entity.getAchetypeCollectionIndex());
	}

	void EntityArchetypeCollection::transferEntity(Entity & entity, EntityArchetypeCollection & destination)
	{
		unsigned int distinationIndex = destination.allocate();

		for (auto & pair : m_componentCollections)
			pair.second->transferComponent(entity.getAchetypeCollectionIndex(), destination.m_componentCollections[pair.first], distinationIndex);

		entity.m_archetypeHashCode = destination.m_archetypeHashCode;
		entity.m_archetypeCollectionIndex = distinationIndex;
	}

	unsigned int EntityArchetypeCollection::getEntityCount() const
	{
		if (m_componentCollections.size() == 0)
			return 0;

		return m_componentCollections.begin()->second->getCount();
	}
}