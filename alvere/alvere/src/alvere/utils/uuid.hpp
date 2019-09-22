#pragma once

#include <string>

namespace alvere
{
	class UUID
	{
	public:

		static const UUID k_invalidUUID;

		static UUID create();

		std::string toString() const;

		inline bool operator==(const UUID & rhs) const
		{
			for (int i = 0; i < 16; ++i)
				if (m_bytes[i] != rhs.m_bytes[i])
					return false;
			return true;
		}

		inline bool operator!=(const UUID & rhs) const
		{
			return !operator==(rhs);
		}

		inline bool operator<(const UUID & rhs) const
		{
			for (int i = 0; i < 16; ++i)
				if (m_bytes[i] < rhs.m_bytes[i])
					return true;
				else if (m_bytes[i] > rhs.m_bytes[i])
					return false;
			return false;
		}

		std::size_t createHash() const;

	private:

		UUID();

		unsigned char m_bytes[16] = {};
	};
}

namespace std
{
	template<>
	struct hash<alvere::UUID>
	{
		std::size_t operator()(const alvere::UUID & uuid) const
		{
			return uuid.createHash();
		}
	};
}