#pragma once

#include "alvere/world/archetype/version_map.hpp"

namespace alvere
{
	class Archetype;

	struct Entity
	{
		Archetype * m_Archetype;
		VersionMap::Handle m_MappingHandle;
	};
}