#pragma once

#include <alvere/world/world.hpp>

#include <alvere/world/entity/entity.hpp>
#include <alvere/world/entity/entity_handle.hpp>

class EntityDefinition
{
public:

	EntityDefinition() = default;
	virtual ~EntityDefinition() = default;

	virtual alvere::EntityHandle SpawnInstance( alvere::World & world ) = 0;
};