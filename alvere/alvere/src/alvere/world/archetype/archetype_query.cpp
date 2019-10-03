#include "alvere/world/archetype/archetype_query.hpp"

bool Archetype::Query::Matches( const Archetype& archetype ) const
{
	int foundTypes = 0;

	for ( auto& provider : archetype.m_Providers )
	{
		MatchType match = Matches( provider.first );

		if ( match == MatchType::Excluded )
		{
			return false;
		}
		else if ( match == MatchType::Included )
		{
			foundTypes += 1;
		}
	}

	return foundTypes == m_IncludedTypes.size();
}

Archetype::Query::MatchType Archetype::Query::Matches( const std::type_index& providerType ) const
{
	for ( auto& includedType : m_IncludedTypes )
	{
		if ( providerType == includedType )
		{
			return MatchType::Included;
		}
	}

	for ( auto& excludedType : m_ExcludedTypes )
	{
		if ( providerType == excludedType )
		{
			return MatchType::Excluded;
		}
	}

	return MatchType::Ignored;
}