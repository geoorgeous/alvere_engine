#pragma once

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <string>
#include <vector>

//#include "alvere/utils/exceptions.hpp"
#include "alvere/world/component/component_provider.hpp"
#include "alvere/world/entity/entity.hpp"
#include "alvere/world/archetype/version_map.hpp"

namespace alvere
{
	class Archetype
	{
		std::unordered_map<std::type_index, ComponentProvider*> m_Providers;

		VersionMap m_VersionMap;

		std::size_t m_EntityCount;

	public:
		class Query;
		class Handle;
		Handle GetHandle() const;

		Archetype();

		template <typename T>
		T& GetComponent( const Entity& entity );

		void AddEntity( Entity& entity );
		void DestroyEntity( Entity& entity );
		void MoveEntity( Entity& entity, Archetype& other );

		void CloneAllProviders( const Archetype& other );
		void CloneAllProvidersExcept( const Archetype& other, const std::type_index& exception );

		template <typename T>
		void AddProvider();

		template <typename T>
		typename T::Provider& GetProvider();

		std::size_t GetEntityCount() const;
		std::size_t GetProviderCount() const;
		std::vector<std::string> GetProviders() const;

		template <typename... Components>
		static Archetype* make_archetype();
	};

	template <typename T>
	T& Archetype::GetComponent( const Entity& entity )
	{
		auto iter = m_Providers.find( typeid( T ) );

		//AlvWasAssert( iter != m_Providers.end(), "[Archetype] Attempted to get a component provider that is not contained in this archetype" );

		int mappedIndex = (int) m_VersionMap.GetMapping( entity.m_MappingHandle );
		typename T::Provider* typedProvider = static_cast<typename T::Provider*>( iter->second );
		return static_cast<T&>( typedProvider->GetComponent( mappedIndex ) );
	}

	template <typename T>
	void Archetype::AddProvider()
	{
		//AlvWasAssert( m_Providers.find( typeid( T ) ) == m_Providers.end(), "[Archetype] Attempted to add a provider we already contain" );
		m_Providers.emplace( typeid( T ), new typename T::Provider() );
	}

	template <typename T>
	typename T::Provider& Archetype::GetProvider()
	{
		//AlvWasAssert( m_Providers.find( typeid( T ) ) != m_Providers.end(), "[Archetype] Attmpted to get a provider not in this archetype" );
		return static_cast<typename T::Provider&>( *m_Providers[ typeid( T ) ] );
	}

	template <typename... Components>
	static Archetype * Archetype::make_archetype()
	{
		Archetype * archetype = new Archetype();
		(archetype->AddProvider<Components>(), ...);
		return archetype;

	}
}

#include "alvere/world/archetype/archetype_query.hpp"
