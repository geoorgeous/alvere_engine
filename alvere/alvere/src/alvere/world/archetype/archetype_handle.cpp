#include "alvere/world/archetype/archetype_handle.hpp"

namespace alvere
{
	void Archetype::Handle::AddComponent(const std::type_index & type)
	{
		auto iter = std::find(m_Types.begin(), m_Types.end(), type);

		if (iter != m_Types.end())
		{
			//TODO: Log a warning that the component already exists
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
			//TODO: Log a warning that the component didn't exist
			return;
		}

		m_Hash ^= type.hash_code();
		m_Types.erase(iter);
	}
}