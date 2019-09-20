#include "alvere/world/entity_component_map.hpp"

namespace alvere
{
	EntityComponentMap::~EntityComponentMap()
	{
		for (auto & iter : m_entityComponentCollections)
		{
			delete iter.second;
		}
	}
}