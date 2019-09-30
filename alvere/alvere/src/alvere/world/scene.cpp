#include "alvere/world/scene.hpp"

#include "alvere/world/entity_components/ec_transform.hpp"

namespace alvere
{
	Entity Scene::createEntity()
	{
		Entity entity;

		std::type_index transformComponentTypeIndex = typeid(ECTransform);

		EntityArchetypeHashCode transformArchetypeHashCode = std::hash<std::type_index>()(transformComponentTypeIndex);

		auto iter = m_entityArchetypeCollections.find(transformArchetypeHashCode);

		if (iter == m_entityArchetypeCollections.end())
		{
			iter = m_entityArchetypeCollections.emplace(transformArchetypeHashCode, EntityArchetypeCollection()).first;
			iter->second.addCollectionType(transformComponentTypeIndex, new EntityComponentCollection<ECTransform>);
		}

		entity.m_archetypeHashCode = transformArchetypeHashCode;

		entity.m_archetypeCollectionIndex = iter->second.m_componentCollections[transformComponentTypeIndex]->allocate();

		return entity;
	}

	void Scene::destroyEntity(Entity & entity)
	{
		m_entityArchetypeCollections[entity.getArchetypeHashCode()].removeEntity(entity);
	}

	void Scene::update(float deltaTime)
	{
		for (auto & pair : m_updatedSystems)
			pair.second->update(deltaTime);
	}

	void Scene::draw()
	{
		for (auto & pair : m_drawableSystems)
			pair.second->draw();
	}
}