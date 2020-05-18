#include "alvere/world/archetype/archetype_handle.hpp"
#include "alvere/debug/exceptions.hpp"

namespace alvere
{
	void Archetype::Handle::AddComponent(const std::type_index & type)
	{
		auto iter = std::find(m_Types.begin(), m_Types.end(), type);

		if (iter != m_Types.end())
		{
			LogWarning( "[Archetype::Handle] Cannot add component as it already exists on this entity" );
			return;
		}

		m_Hash ^= type.hash_code();
		m_Types.emplace_back(type);
	}

	void Archetype::Handle::RemoveComponent(const std::type_index & type)
	{
		auto iter = std::find(m_Types.begin(), m_Types.end(), type);

		if (iter == m_Types.end())
		{
			LogWarning( "[Archetype::Handle] Cannot remove component as it didn't exist on this entity" );
			return;
		}

		m_Hash ^= type.hash_code();
		m_Types.erase(iter);
	}

	const std::vector<std::type_index> & Archetype::Handle::GetTypes() const
	{
		return m_Types;
	}
}