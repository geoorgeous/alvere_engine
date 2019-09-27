#include "alvere/world/entity_component_collection.hpp"

namespace alvere
{
	/*template <typename EntityComponentType>
	void EntityComponentCollection<EntityComponentType>::transferComponent(unsigned int index, EntityComponentCollectionBase * destination, unsigned int destinationIndex)
	{
		EntityComponentCollection<EntityComponentType> dest = *static_cast<EntityComponentCollection<EntityComponentType> *>(destination);

		auto sourceIter = m_components.begin() + index;
		auto destIter = dest.m_components.begin() + destinationIndex;

		*destIter = std::move(sourceIter);

		*sourceIter = std::move(m_components.back());
		m_components.pop_back();
	}*/
}