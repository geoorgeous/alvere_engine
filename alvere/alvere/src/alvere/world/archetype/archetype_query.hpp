#pragma once

#include <typeindex>
#include <vector>

#include "alvere/world/archetype/archetype.hpp"

namespace alvere
{
	class Archetype::Query
	{
		enum class MatchType
		{
			Included,
			Excluded,
			Ignored,
		};

	public:

		std::vector<std::type_index> m_IncludedTypes;
		std::vector<std::type_index> m_ExcludedTypes;

		//This assumes there are no duplicate types in either the archetype or the included vector
		bool Matches(const Archetype & archetype) const;

		template <typename... Components>
		Archetype::Query & Include();

		template <typename... Components>
		Archetype::Query & Exclude();

	private:

		MatchType Matches(const std::type_index & providerType) const;
	};

	template <typename... Components>
	Archetype::Query & Archetype::Query::Include()
	{
		(m_IncludedTypes.emplace_back(typeid(Components)), ...);
		return *this;
	}

	template <typename... Components>
	Archetype::Query & Archetype::Query::Exclude()
	{
		(m_ExcludedTypes.emplace_back(typeid(Components)), ...);
		return *this;
	}
}