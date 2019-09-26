#pragma once

#include <vector>

namespace alvere
{
	class EntityComponentCollectionBase
	{
	public:

		virtual EntityComponentCollectionBase getEmptyClone() const;
	};

	template <typename EntityComponentType>
	class EntityComponentCollection : public EntityComponentCollectionBase
	{
	public:

		EntityComponentCollection<EntityComponentType> getEmptyClone() const override
		{
			return EntityComponentCollection<EntityComponentType>();
		}

	private:

		std::vector<EntityComponentType> m_components;
	};
}