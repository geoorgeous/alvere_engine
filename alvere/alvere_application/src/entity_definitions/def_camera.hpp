#pragma once

#include "entity_definition.hpp"

class Def_Camera : EntityDefinition
{
public:

	virtual alvere::EntityHandle SpawnInstance(alvere::World & world) override;
};