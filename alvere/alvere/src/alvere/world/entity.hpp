#pragma once

#include "alvere/utils/uuid.hpp"
#include "alvere/world/entity_archetype_collection.hpp"
#include "alvere/world/transform.hpp"

namespace alvere
{
	class Entity
	{
	public:

		friend class Scene;

		friend class EntityArchetypeCollection;

		Entity();

		inline EntityArchetypeHashCode getArchetypeHashCode() const
		{
			return m_archetypeHashCode;
		}

		inline unsigned int getAchetypeCollectionIndex() const
		{
			return m_archetypeCollectionIndex;
		}

		inline const Transform & getTransform() const
		{
			return *m_transform;
		}

		inline Transform & transform()
		{
			return *m_transform;
		}

	private:

		EntityArchetypeHashCode m_archetypeHashCode;

		unsigned int m_archetypeCollectionIndex;

		Transform * m_transform;
	};
}