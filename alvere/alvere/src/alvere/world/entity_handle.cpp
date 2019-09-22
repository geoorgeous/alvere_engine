#include "alvere/world/entity_handle.hpp"

#include "alvere/world/entity.hpp"
#include "alvere/world/scene.hpp"

namespace alvere
{
	EntityHandle::EntityHandle()
		: m_entityUUID(UUID::k_invalidUUID), m_scene(nullptr)
	{ }

	EntityHandle::EntityHandle(const Entity & entity, Scene & scene)
		: m_entityUUID(entity.getUUID()), m_scene(&scene)
	{ }

	Entity * EntityHandle::get()
	{
		if (!isValid())
			return false;

		return &m_scene->m_entities[m_entityUUID];
	}

	const Entity * EntityHandle::get() const
	{
		if (!isValid())
			return false;

		return &m_scene->m_entities.at(m_entityUUID);
	}
}