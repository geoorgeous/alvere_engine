#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <type_traits>

#include "alvere/world/archetype/archetype.hpp"
#include "alvere/world/archetype/archetype_handle.hpp"
#include "alvere/world/archetype/archetype_query.hpp"
#include "alvere/world/entity/entity.hpp"

namespace alvere
{
	class UpdatedSystem;
	class RenderedSystem;

	class World
	{
		std::unordered_map<Archetype::Handle, Archetype *> m_Archetypes;

		std::unordered_map<std::type_index, UpdatedSystem *> m_UpdatedSystems;

		std::unordered_map<std::type_index, RenderedSystem *> m_RenderedSystems;

	public:

		World();

		void Update(float deltaTime);

		void Render();

		Entity SpawnEntity();

		template <typename... Components>
		Entity SpawnEntity();

		void DestroyEntity(Entity & entity);

		template <typename T>
		void AddComponent(Entity & e);

		template <typename T>
		void RemoveComponent(Entity & e);

		template <typename T>
		T & GetComponent(const Entity & e) const;

		template <typename T>
		T * AddSystem();

		template <typename T>
		void RemoveSystem();

		void QueryArchetypes(const Archetype::Query & query, std::vector<std::reference_wrapper<Archetype>> & matchingArchetypes) const;
	};

	template <typename... Components>
	Entity World::SpawnEntity()
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

		Entity e;
		archetype->AddEntity(e);
		return e;
	}

	template <typename T>
	void World::AddComponent(Entity & entity)
	{
		Archetype * originalArchetype = entity.m_Archetype;

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
	void World::RemoveComponent(Entity & entity)
	{
		Archetype * originalArchetype = entity.m_Archetype;

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
	T & World::GetComponent(const Entity & e) const
	{
		return e.m_Archetype->GetComponent<T>(e);
	}

	template <typename T>
	T * World::AddSystem()
	{
		T * t = new T();
		bool added = false;

		if (std::is_base_of<UpdatedSystem, T>::value)
		{
			added |= m_UpdatedSystems.emplace(typeid(T), (UpdatedSystem *)(t)).second;
		}

		if (std::is_base_of<RenderedSystem, T>::value)
		{
			added |= m_RenderedSystems.emplace(typeid(T), (RenderedSystem *)(t)).second;
		}

		if (added == false)
		{
			delete t;
		}

		return t;
	}

	template <typename T>
	void World::RemoveSystem()
	{
		T * system = nullptr;

		if (std::is_base_of<UpdatedSystem, T>::value)
		{
			auto iter = m_UpdatedSystems.find(typeid(T));

			if (iter != m_UpdatedSystems.end())
			{
				system = (T *)iter->second;
				m_UpdatedSystems.erase(iter);
			}
		}

		if (std::is_base_of<RenderedSystem, T>::value)
		{
			auto iter = m_RenderedSystems.find(typeid(T));

			if (iter != m_RenderedSystems.end())
			{
				system = (T *)iter->second;
				m_RenderedSystems.erase(iter);
			}
		}

		if (system != nullptr)
		{
			delete system;
		}
	}
}