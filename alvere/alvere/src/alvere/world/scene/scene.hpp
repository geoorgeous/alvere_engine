#pragma once

#include <vector>

#include "alvere/world/entity/entity_handle.hpp"

namespace alvere
{
	class Scene
	{
	public:

		std::vector<EntityHandle> m_Entities;

		void AddEntity(EntityHandle & entity);

		void RemoveEntity(EntityHandle & entity);
	};
}