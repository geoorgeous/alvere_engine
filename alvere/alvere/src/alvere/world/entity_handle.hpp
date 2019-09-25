#pragma once

#include "alvere/world/entity.hpp"

namespace alvere
{
	class EntityHandle
	{
	public:

		inline bool isValid() const
		{
			return false;
		}

		inline Entity * get() const
		{
			return nullptr;
		}

	private:
	};
}