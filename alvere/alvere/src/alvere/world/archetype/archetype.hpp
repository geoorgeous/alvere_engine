#pragma once

#include <unordered_map>
#include <unordered_set>
#include <typeinfo>
#include <typeindex>
#include <string>
#include <vector>

#include "alvere/debug/exceptions.hpp"
#include "alvere/world/component/component_provider.hpp"
#include "alvere/world/entity/entity.hpp"
#include "alvere/world/entity/entity_handle.hpp"
#include "alvere/world/archetype/version_map.hpp"

namespace alvere
{
	class Archetype
	{
		std::unordered_map<std::type_index, ComponentProvider*> m_Providers;
		std::unordered_set<EntityHandle, EntityHandle::Hash> m_Entities;

		VersionMap m_VersionMap;

	public:

		~Archetype();

		class Query;
		class Handle;
		Handle GetHandle() const;

		template <typename T>
		T& GetComponent( const EntityHandle& entity );

		void AddEntity(EntityHandle & entity );
		void DestroyEntity(EntityHandle & entity );
		void MoveEntity(EntityHandle & entity, Archetype& other );

		void CloneAllProviders( const Archetype& other );
		void CloneAllProvidersExcept( const Archetype& other, const std::type_index& exception );

		template <typename T>
		void AddProvider();

		template <typename T>
		typename T::Provider& GetProvider();

		const std::unordered_set<EntityHandle, EntityHandle::Hash> & GetEntities() const;

		std::size_t GetEntityCount() const;
		std::size_t GetProviderCount() const;

		template <typename... Components>
		static Archetype* make_archetype();
	};

	template <typename T>
	T& Archetype::GetComponent( const EntityHandle & entity )
	{
		auto iter = m_Providers.find( typeid( T ) );

		AlvAssert( iter != m_Providers.end(), "Attempted to get a component provider that is not contained in this archetype" );

		int mappedIndex = (int) m_VersionMap.GetMapping( entity->m_MappingHandle );
		typename T::Provider* typedProvider = static_cast<typename T::Provider*>( iter->second );
		return static_cast<T&>( typedProvider->GetComponent( mappedIndex ) );
	}

	template <typename T>
	void Archetype::AddProvider()
	{
		AlvAssert( m_Providers.find( typeid( T ) ) == m_Providers.end(), "Attempted to add a provider we already contain" );
		m_Providers.emplace( typeid( T ), new typename T::Provider() );
	}

	template <typename T>
	typename T::Provider& Archetype::GetProvider()
	{
		AlvAssert( m_Providers.find( typeid( T ) ) != m_Providers.end(), "Attmpted to get a provider not in this archetype" );
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
