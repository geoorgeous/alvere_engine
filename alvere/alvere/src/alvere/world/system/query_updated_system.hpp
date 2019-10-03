#pragma once

#include "alvere/world/system/updated_system.hpp"
#include "alvere/world/archetype/archetype.hpp"
#include "alvere/world/archetype/archetype_query.hpp"
#include "alvere/world/archetype/archetype_provider_iterator.hpp"

namespace alvere
{
	template <typename... Components>
	class QueryUpdatedSystem : public UpdatedSystem
	{
		std::vector<std::reference_wrapper<Archetype>> m_Archetypes;
		Archetype::Query m_UpdateQuery;

	public:

		QueryUpdatedSystem()
			: m_UpdateQuery(Archetype::Query().Include<Components...>())
		{
		}

		virtual void Update(World & world, float deltaTime) override
		{
			world.QueryArchetypes(m_UpdateQuery, m_Archetypes);

			for (std::size_t i = 0; i < m_Archetypes.size(); ++i)
			{
				Archetype & archetype = m_Archetypes[i].get();

				ArchetypeProviderIterator<Components... > iterator(archetype.GetEntityCount(), archetype.GetProvider<Components>()...);
				for (; iterator; ++iterator)
				{
					std::apply([this](auto && ... args) { Update(args...); }, iterator.GetComponents());
				}
			}
		}

		virtual void Update(Components & ...) = 0;
	};
}