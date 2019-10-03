#pragma once

#include <tuple>
#include <functional>

template <typename... Components>
class ArchetypeProviderIterator
{
public:
	std::size_t m_EntityCount;
	std::tuple<typename Components::Provider::iterator...> m_Iterators;

	ArchetypeProviderIterator( std::size_t entityCount, typename Components::Provider&... providers )
		: m_EntityCount( entityCount )
		, m_Iterators( providers.begin()... )
	{
	}

	ArchetypeProviderIterator& operator++()
	{
		m_EntityCount -= 1;
		std::apply( &ArchetypeProviderIterator::IncrementIterators, m_Iterators );
		return *this;
	}

	operator bool() const
	{
		return m_EntityCount > 0;
	}

	static void IncrementIterators( typename Components::Provider::iterator&... iterators )
	{
		( ++iterators, ... );
	}

	std::tuple<std::reference_wrapper<Components>...> GetComponents()
	{
		return std::apply( &ArchetypeProviderIterator::make_components, m_Iterators );
	}

	static std::tuple<std::reference_wrapper<Components>...> make_components( typename Components::Provider::iterator&... iterators )
	{
		return std::tuple<std::reference_wrapper<Components>...>( *iterators... );
	}
};