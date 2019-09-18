#include <algorithm>

#include "alvere/world/scene.hpp"

#include "alvere/world/entity_component_systems/ecs_scene_renderer.hpp"

namespace alvere
{
	Scene::Scene()
	{
		addSystem(&m_sceneRenderer);
	}

	Entity * Scene::createEntity()
	{
		Entity newEntity = Entity(UUID::create());

		m_entities.push_back(newEntity);

		return &m_entities.back();
	}
	
	void Scene::destroyEntity(Entity * entity)
	{
		m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), *entity), m_entities.end());
	}

	void Scene::setEntityParent(Entity * entity, Entity * parent)
	{
		Transform * t = nullptr;

		if (parent != nullptr)
		{
			t = &parent->transform().transform;
		}

		entity->transform().transform.setParent(t);
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
			m_systems[i]->updateSystems(m_components, timeStep);
		}
	}
}