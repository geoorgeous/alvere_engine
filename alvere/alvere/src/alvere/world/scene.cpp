#include "alvere/world/scene.hpp"

namespace alvere
{
	void Scene::update(float timeStep)
	{
		for (int i = 0; i < m_systems.size(); ++i)
		{
			m_systems[i].update(m_components, timeStep);
		}
	}

	Entity * Scene::createEntity()
	{
		Entity newEntity = Entity(UUID::create());

		m_entities.push_back(newEntity);

		return &m_entities.back();
	}
	
	void Scene::destroyEntity(Entity * entity)
	{

	}
}