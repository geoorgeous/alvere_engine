#pragma once

#include <typeindex>
#include <unordered_map>

#include "alvere/world/entity_component.hpp"
#include "alvere/world/entity_components/ec_transform.hpp"
#include "alvere/utils/uuid.hpp"

namespace alvere
{
	class Scene;

	class EntityHandle
	{
	public:

		EntityHandle(int index, Scene & scene)
			: m_entityIndex(index), m_scene(scene)
		{ }

		inline void invalidate()
		{
			m_entityIndex = k_invalidEntityIndex;
		}

		inline const Entity * get() const
		{
			return &m_scene.m_entities[m_entityIndex];
		}

		inline Entity * get()
		{
			return &m_scene.m_entities[m_entityIndex];
		}

		inline bool isValid() const
		{
			return m_entityIndex != k_invalidEntityIndex;
		}

	private:

		const int k_invalidEntityIndex = -1;

		int m_entityIndex = k_invalidEntityIndex;

		Scene & m_scene;
	};

	class Entity
	{
	public:

		Entity(UUID guid, Scene & scene);

		inline ECTransform & transform()
		{
			return *m_transform;
		}

		inline const ECTransform & transform() const
		{
			return *m_transform;
		}

		inline bool operator==(const Entity & rhs) const
		{
			return m_uuid == rhs.m_uuid;
		}

	private:

		UUID m_uuid;

		ECTransform * m_transform;

		std::unordered_map<std::type_index, EntityComponent *> m_components;
	};
}