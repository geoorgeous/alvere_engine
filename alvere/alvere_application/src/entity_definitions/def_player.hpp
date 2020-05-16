#pragma once

#include "entity_definition.hpp"

class Def_Player : EntityDefinition
{
public:

	virtual alvere::EntityHandle SpawnInstance(alvere::World & world) override;
};