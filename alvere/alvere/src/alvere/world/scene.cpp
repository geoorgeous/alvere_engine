#include "alvere/world/scene.hpp"

#include <algorithm>

#include "alvere/world/entity_component_system.hpp"

namespace alvere
{
	Scene::Scene()
	{
		addSystem(&m_sceneRenderer);
	}

	EntityHandle Scene::createEntity()
	{
		UUID newEntityUUID = UUID::create();

		m_entities[newEntityUUID] = Entity(newEntityUUID, *this);

		return EntityHandle(m_entities[newEntityUUID], *this);
	}
	
	void Scene::destroyEntity(EntityHandle & entity)
	{
		if (!entity.isValid())
			return;

		m_entities.erase(entity.m_entityUUID);

		entity.invalidate();
	}

	void Scene::setEntityParent(EntityHandle & entity, EntityHandle & parent)
	{
		if (!entity.isValid() || !parent.isValid())
			return;

		Transform *	t = &parent.get()->transform();

		entity.get()->transform().setParent(t);
	}

	void Scene::orphanEntity(EntityHandle & entity)
	{
		if (!entity.isValid())
			return;

		entity.get()->transform().setParent(nullptr);
	}

	void Scene::addSystem(EntityComponentSystem * system)
	{
		m_systems.push_back(system);
	}

	void Scene::removeSystem(EntityComponentSystem * system)
	{
		m_systems.erase(std::remove(m_systems.begin(), m_systems.end(), system), m_systems.end());
	}

	void Scene::updateSystems(float timeStep)
	{
		for (int i = 0; i < m_systems.size(); ++i)
		{
			m_systems[i]->updateSystems(m_entityComponentMap, timeStep);
		}
	}
}