#pragma once

#include "alvere/world/system/rendered_system.hpp"
#include "alvere/world/archetype/archetype.hpp"
#include "alvere/world/archetype/archetype_query.hpp"
#include "alvere/world/archetype/archetype_provider_iterator.hpp"

namespace alvere
{
	template <typename... Components>
	class QueryRenderedSystem : public RenderedSystem
	{
		std::vector<std::reference_wrapper<Archetype>> m_Archetypes;
		Archetype::Query m_RenderQuery;

	public:

		QueryRenderedSystem()
			: m_RenderQuery(Archetype::Query().Include<Components...>())
		{
		}

		virtual void Render(World & world) override
		{
			world.QueryArchetypes(m_RenderQuery, m_Archetypes);

			for (std::size_t i = 0; i < m_Archetypes.size(); ++i)
			{
				Archetype & archetype = m_Archetypes[i].get();

				ArchetypeProviderIterator<Components... > iterator(archetype.GetEntityCount(), archetype.GetProvider<Components>()...);
				for (; iterator; ++iterator)
				{
					std::apply([this](auto && ... args) { Render(args...); }, iterator.GetComponents());
				}
			}
		}

		virtual void Render(Components & ...) = 0;
	};
}