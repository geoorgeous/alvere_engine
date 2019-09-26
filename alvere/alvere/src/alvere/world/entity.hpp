#pragma once

#include "alvere/utils/uuid.hpp"
#include "alvere/world/entity_archetype.hpp"
#include "alvere/world/transform.hpp"

namespace alvere
{
	class Entity
	{
	public:

		inline EntityArchetype getArchetype() const
		{
			return m_archetype;
		}

	private:

		UUID m_uuid;

		EntityArchetype m_archetype;

		Transform * m_transform;
	};
}