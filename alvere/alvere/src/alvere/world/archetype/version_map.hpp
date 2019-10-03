#pragma once

#include <vector>

//#include "alvere/utils/exceptions.hpp"

class VersionMap
{
	struct Map
	{
		Map( std::size_t index )
			: m_Index( index )
			, m_Version( 0 )
			, m_NextFree( -1 )
		{
		}

		std::size_t m_Index;
		int m_Version;
		std::size_t m_NextFree;
	};

	std::vector<Map> m_Mappings;
	std::size_t m_FirstFree;

public:

	struct Handle
	{
		std::size_t m_Index;
		int m_Version;
	};

	VersionMap()
		: m_FirstFree( -1 )
	{
	}

	Handle AddMapping( std::size_t toIndex )
	{
		if ( m_FirstFree == -1 )
		{
			//Nothing left in the pool, must add a new mapping
			m_Mappings.emplace_back( toIndex );
			return { m_Mappings.size() - 1, 0 };
		}
		
		//Grab a free index to use, moving the first free along
		std::size_t indexToUse = m_FirstFree;
		m_FirstFree = m_Mappings[ indexToUse ].m_NextFree;

		//Assign the given data to the new mapping
		Map& toUse = m_Mappings[ indexToUse ];
		toUse.m_Index = toIndex;
		toUse.m_NextFree = -1;

		return { indexToUse, toUse.m_Version };
	}

	void RemoveMapping( Handle handle )
	{
		Map& toRemove = m_Mappings[ handle.m_Index ];

		//Update the mapping for the valid element that will be swapped
		std::size_t swapIndex = m_Mappings.size() - 1;
		for ( Map& mapping : m_Mappings )
		{
			if ( mapping.m_Index == swapIndex )
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
	}

	std::size_t GetMapping( const Handle& handle ) const
	{
		//AlvWasAssert( IsMappingValid( handle ), "[VersionMap] Must check IsMappingValid() before attempting to get the mapping" );
		return m_Mappings[ handle.m_Index ].m_Index;
	}

	bool IsMappingValid( const Handle& handle ) const
	{
		return handle.m_Version == m_Mappings[ handle.m_Index ].m_Version;
	}
};