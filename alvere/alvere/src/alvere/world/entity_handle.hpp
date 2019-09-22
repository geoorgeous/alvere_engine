#pragma once

#include "alvere/utils/uuid.hpp"

namespace alvere
{
	class Entity;

	class Scene;

	class EntityHandle
	{
	public:

		friend class Scene;

		EntityHandle();

		EntityHandle(const Entity & entity, Scene & scene);

		Entity * get();

		const Entity * get() const;

		inline bool isValid() const
		{
			return m_entityUUID != UUID::k_invalidUUID;
		}

		inline void invalidate()
		{
			m_entityUUID = UUID::k_invalidUUID;
		}

	private:

		UUID m_entityUUID = UUID::k_invalidUUID;

		Scene * m_scene;
	};
}