#include "alvere/world/world.hpp"
#include "alvere/world/archetype/version_map.hpp"
#include "alvere/world/system/updated_system.hpp"
#include "alvere/world/system/rendered_system.hpp"

World::World()
{
	m_Archetypes.emplace( Archetype::Handle::make_handle<>(), new Archetype() );
}

void World::Update( float deltaTime )
{
	for ( auto& system : m_UpdatedSystems )
	{
		system.second->Update( *this, deltaTime );
	}
}

void World::Render()
{
	for ( auto& system : m_RenderedSystems )
	{
		system.second->Render( *this );
	}
}

Entity World::SpawnEntity()
{
	Entity e;
	m_Archetypes[ Archetype::Handle::make_handle<>() ]->AddEntity( e );
	return e;
}

void World::DestroyEntity( Entity& entity )
{
	Archetype* archetype = entity.m_Archetype;
	archetype->DestroyEntity( entity );
}

void World::QueryArchetypes( const Archetype::Query& query, std::vector<std::reference_wrapper<Archetype>>& matchingArchetypes ) const
{
	matchingArchetypes.clear();

	for ( auto& archetype : m_Archetypes )
	{
		if ( query.Matches( *archetype.second ) )
		{
			matchingArchetypes.emplace_back( *archetype.second );
		}
	}
}