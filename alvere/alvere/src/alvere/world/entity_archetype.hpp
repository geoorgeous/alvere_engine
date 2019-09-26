#pragma once

#include <typeindex>
#include <vector>

namespace alvere
{
	class EntityArchetype
	{
	public:

		EntityArchetype(const std::vector<std::type_index> & entityComponentTypes);

		inline std::size_t getHash() const
		{
			return m_hash;
		}

	private:

		std::size_t m_hash;
	};
}