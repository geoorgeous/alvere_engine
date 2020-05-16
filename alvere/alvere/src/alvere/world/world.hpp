#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <type_traits>

#include "alvere/world/system/system.hpp"
#include "alvere/world/archetype/archetype.hpp"
#include "alvere/world/archetype/archetype_handle.hpp"
#include "alvere/world/archetype/archetype_query.hpp"
#include "alvere/world/entity/entity.hpp"
#include "alvere/world/entity/entity_handle.hpp"
#include "alvere/utils/pool.hpp"

namespace alvere
{
	class UpdatedSystem;
	class RenderedSystem;

	class World
	{
		std::unordered_map<Archetype::Handle, Archetype *> m_Archetypes;

		std::unordered_map<std::type_index, System *> m_AllSystems;
		std::unordered_map<std::type_index, UpdatedSystem *> m_UpdatedSystems;
		std::unordered_map<std::type_index, RenderedSystem *> m_RenderedSystems;

		Pool<Entity> m_Entities;

	public:

		World();
		~World();

		void Update(float deltaTime);

		void Render();

		EntityHandle SpawnEntity();

		template <typename... Components>
		EntityHandle SpawnEntity();

		void DestroyEntity(EntityHandle & entity);

		template <typename T>
		void AddComponent(EntityHandle & e);

		template <typename T>
		void RemoveComponent(EntityHandle & e);

		template <typename T>
		T & GetComponent(const EntityHandle & e) const;

		template <typename T, typename... Args>
		T * AddSystem( Args&&... args );

		template <typename T>
		T * GetSystem();

		template <typename T>
		void RemoveSystem();

		void QueryArchetypes(const Archetype::Query & query, std::vector<std::reference_wrapper<Archetype>> & matchingArchetypes) const;
	};

	template <typename... Components>
	EntityHandle World::SpawnEntity()
	{
		Archetype::Handle archetypeHandle = Archetype::Handle::make_handle<Components...>();

		Archetype * archetype;
		auto iter = m_Archetypes.find(archetypeHandle);
		if (iter != m_Archetypes.end())
		{
			archetype = iter->second;
		}
		else
		{
			archetype = Archetype::make_archetype<Components...>();
			m_Archetypes[archetypeHandle] = archetype;
		}

		EntityHandle e = m_Entities.allocate();
		archetype->AddEntity(e);
		return e;
	}

	template <typename T>
	void World::AddComponent(EntityHandle & entity)
	{
		Archetype * originalArchetype = entity->m_Archetype;

		Archetype::Handle newArchetypeHandle = originalArchetype->GetHandle();
		newArchetypeHandle.AddComponent<T>();

		Archetype * newArchetype;

		auto iter = m_Archetypes.find(newArchetypeHandle);
		if (iter != m_Archetypes.end())
		{
			newArchetype = iter->second;
		}
		else
		{
			newArchetype = new Archetype();
			newArchetype->CloneAllProviders(*originalArchetype);
			newArchetype->AddProvider<T>();

			m_Archetypes[newArchetypeHandle] = newArchetype;
		}

		originalArchetype->MoveEntity(entity, *newArchetype);
	}

	template <typename T>
	void World::RemoveComponent(EntityHandle & entity)
	{
		Archetype * originalArchetype = entity->m_Archetype;

		Archetype::Handle newArchetypeHandle = originalArchetype->GetHandle();
		newArchetypeHandle.RemoveComponent<T>();

		Archetype * newArchetype;

		auto iter = m_Archetypes.find(newArchetypeHandle);
		if (iter != m_Archetypes.end())
		{
			newArchetype = iter->second;
		}
		else
		{
			newArchetype = new Archetype();
			newArchetype->CloneAllProvidersExcept(*originalArchetype, typeid(T));

			m_Archetypes[newArchetypeHandle] = newArchetype;
		}

		originalArchetype->MoveEntity(entity, *newArchetype);
	}

	template <typename T>
	T & World::GetComponent(const EntityHandle & e) const
	{
		return e->m_Archetype->GetComponent<T>(e);
	}

	template <typename T, typename... Args>
	T * World::AddSystem( Args&&... args )
	{
		std::type_index systemType = typeid( T );

		auto iter = m_AllSystems.find( systemType );
		if ( iter != m_AllSystems.end() )
		{
			return dynamic_cast<T*>(iter->second);
		}

		T * t = new T( std::forward<Args...>( args )... );
		m_AllSystems[ systemType ] = t;


		if (std::is_base_of<UpdatedSystem, T>::value)
		{
			m_UpdatedSystems.emplace( systemType, (UpdatedSystem *) t );
		}

		if (std::is_base_of<RenderedSystem, T>::value)
		{
			m_RenderedSystems.emplace( systemType, (RenderedSystem *) t );
		}

		return t;
	}

	template <typename T>
	void World::RemoveSystem()
	{
		std::type_index systemType = typeid( T );

		auto allIter = m_AllSystems.find( systemType );
		if (allIter == m_AllSystems.end())
		{
			return;
		}

		if (std::is_base_of<UpdatedSystem, T>::value)
		{
			auto iter = m_UpdatedSystems.find( systemType );

			if (iter != m_UpdatedSystems.end())
			{
				m_UpdatedSystems.erase(iter);
			}
		}

		if (std::is_base_of<RenderedSystem, T>::value)
		{
			auto iter = m_RenderedSystems.find( systemType );

			if (iter != m_RenderedSystems.end())
			{
				m_RenderedSystems.erase(iter);
			}
		}

		delete allIter->second;
		m_AllSystems.erase(allIter);
	}

	template <typename T>
	T * World::GetSystem()
	{
		auto iter = m_AllSystems.find( typeid( T ) );
		return iter != m_AllSystems.end()
			? dynamic_cast<T *>( iter->second )
			: nullptr;
	}
}