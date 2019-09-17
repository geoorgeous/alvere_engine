#pragma once

#include <string>

namespace alvere
{
	class Entity;

	struct EntityComponent
	{
		const Entity & entity;
	};
}