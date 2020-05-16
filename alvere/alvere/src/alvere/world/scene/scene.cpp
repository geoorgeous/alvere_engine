#include "alvere/world/scene/scene.hpp"

namespace alvere
{
	void Scene::AddEntity(const EntityHandle & entity)
	{
		AlvAssert(entity.isValid(), "Invalid entity handle");

		auto iter = std::find(m_Entities.begin(), m_Entities.end(), entity);

		if (iter != m_Entities.end())
		{
			return;
		}

		m_Entities.emplace_back(entity);
	}

	void Scene::RemoveEntity(const EntityHandle & entity)
	{
		AlvAssert(entity.isValid(), "Invalid entity handle");

		auto iter = std::find(m_Entities.begin(), m_Entities.end(), entity);

		if (iter == m_Entities.end())
		{
			return;
		}

		m_Entities.erase(iter);
	}
}