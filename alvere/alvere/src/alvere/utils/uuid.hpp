#pragma once

#include <string>

namespace alvere
{
	class UUID
	{
	public:

		static UUID create();

		std::string toString() const;

		inline bool operator==(const UUID & rhs) const
		{
			for (int i = 0; i < 16; ++i)
				if (m_bytes[i] != rhs.m_bytes[i])
					return false;
			return true;
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

	private:

		UUID();

		unsigned char m_bytes[16];
	};
}