#pragma once

#include <vector>

#include "alvere/debug/exceptions.hpp"

namespace alvere
{
	class VersionMap
	{
		struct Map
		{
			Map(std::size_t index)
				: m_Index(index)
				, m_Version(0)
				, m_NextFree(-1)
			{
			}

			std::size_t m_Index;
			std::size_t m_NextFree;
			int m_Version;
		};

		std::vector<Map> m_Mappings;
		std::size_t m_FirstFree;
		std::size_t m_Count;

	public:

		struct Handle
		{
			std::size_t m_Index;
			int m_Version;
		};

		VersionMap();

		Handle AddMapping(std::size_t toIndex);

		void RemoveMapping(Handle handle);

		void Clear()
		{
			if (m_Count == 0)
			{
				return;
			}

			//Iterate through all mappings and remove any that are still assigned
			//Could potentially re-order the free list to be linear again here but I do not want to mess
			//with increasing versions when it isn't needed.
			for (int i = 0; i < m_Mappings.size(); ++i)
			{
				if (m_Mappings[i].m_Index == -1)
				{
					continue;
				}

				m_Mappings[i].m_Version += 1;
				m_Mappings[i].m_Index = -1;
				m_Mappings[i].m_NextFree = m_FirstFree;
				m_FirstFree = i;
			}

			m_Count = 0;
		}

		std::size_t GetMapping(const Handle & handle) const;

		bool IsMappingValid(const Handle & handle) const;
	};
}