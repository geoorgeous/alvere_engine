#include "alvere/world/scene.hpp"

namespace alvere
{
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