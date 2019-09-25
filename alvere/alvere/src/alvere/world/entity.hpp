#pragma once

#include "alvere/utils/uuid.hpp"
#include "alvere/world/transform.hpp"

namespace alvere
{
	class Entity
	{
	public:

		/*template <typename EntityComponentType>
		EntityComponentType::Handle addComponent()
		{

		}*/

		template <typename EntityComponentType>
		void removeComponent()
		{

		}

	private:

		UUID m_uuid;

		Transform * m_transform;
	};
}