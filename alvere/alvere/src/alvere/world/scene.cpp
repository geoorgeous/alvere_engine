#include "alvere/world/scene.hpp"

namespace alvere
{
	EntityHandle Scene::createEntity()
	{

	}

	void Scene::destroyEntity(EntityHandle & entityHandle)
	{

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