#include "version_map.hpp"

namespace alvere
{
	VersionMap::VersionMap()
		: m_FirstFree(-1)
		, m_Count(0)
	{
	}

	VersionMap::Handle VersionMap::AddMapping(std::size_t toIndex)
	{
		//Increase how many mappings we are currently tracking
		m_Count += 1;

		if (m_FirstFree == -1)
		{
			//Nothing left in the pool, must add a new mapping
			m_Mappings.emplace_back(toIndex);
			return { m_Mappings.size() - 1, 0 };
		}

		//Grab a free index to use, moving the first free along
		std::size_t indexToUse = m_FirstFree;
		m_FirstFree = m_Mappings[indexToUse].m_NextFree;

		//Assign the given data to the new mapping
		Map & toUse = m_Mappings[indexToUse];
		toUse.m_Index = toIndex;
		toUse.m_NextFree = -1;

		return { indexToUse, toUse.m_Version };
	}

	void VersionMap::RemoveMapping(Handle handle)
	{
		Map & toRemove = m_Mappings[handle.m_Index];

		//Update the mapping for the valid element that will be swapped
		std::size_t swapIndex = m_Count - 1;
		for (Map & mapping : m_Mappings)
		{
			if (mapping.m_Index == swapIndex)
			{
				mapping.m_Index = toRemove.m_Index;
				break;
			}
		}

		//Increase the version of the mapping to invalidate existing handles
		toRemove.m_Version += 1;
		toRemove.m_Index = -1;

		//Add this mapping to the free list
		toRemove.m_NextFree = m_FirstFree;
		m_FirstFree = handle.m_Index;

		//Decrease how many mappings we are currently tracking
		m_Count -= 1;
	}

	std::size_t VersionMap::GetMapping(const Handle & handle) const
	{
		AlvAssert(IsMappingValid(handle), "Must check IsMappingValid() before attempting to get the mapping");

		return m_Mappings[handle.m_Index].m_Index;
	}

	bool VersionMap::IsMappingValid(const Handle & handle) const
	{
		return handle.m_Version == m_Mappings[handle.m_Index].m_Version;
	}
}