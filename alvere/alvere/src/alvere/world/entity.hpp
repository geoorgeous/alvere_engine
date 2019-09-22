#pragma once

#include <typeindex>
#include <unordered_map>

#include "alvere/world/entity_component.hpp"
#include "alvere/world/entity_components/ec_transform.hpp"
#include "alvere/utils/uuid.hpp"

namespace alvere
{
	class Scene;

	class Entity
	{
	public:

		Entity();

		Entity(UUID guid, Scene & scene);

		inline const UUID & getUUID() const
		{
			return m_uuid;
		}

		inline Transform & transform()
		{
			return *m_transform;
		}

		inline const Transform & transform() const
		{
			return *m_transform;
		}

		inline bool operator==(const Entity & rhs) const
		{
			return m_uuid == rhs.m_uuid;
		}

	private:

		UUID m_uuid;

		Transform * m_transform;
	};
}