#include "alvere/world/entity.hpp"

#include "alvere/world/scene.hpp"

namespace alvere
{
	Entity::Entity(UUID uuid, Scene & scene)
		: m_uuid(uuid), m_transform(nullptr)
	{
		m_transform = scene.createEntityComponent<ECTransform>(this);
	}
}