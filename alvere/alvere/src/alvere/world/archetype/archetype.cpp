#include <typeindex>

#include "alvere/world/archetype/archetype.hpp"
#include "alvere/world/archetype/archetype_handle.hpp"
#include "alvere/world/entity/entity.hpp"

namespace alvere
{
	void Archetype::AddEntity(EntityHandle & entity)
	{
		for (auto & provider : m_Providers)
		{
			provider.second->Allocate();
		}

		entity->m_Archetype = this;
		entity->m_MappingHandle = m_VersionMap.AddMapping(m_Entities.size());
		m_Entities.emplace(entity);
	}

	void Archetype::DestroyEntity(EntityHandle & entity)
	{
		int mappedIndex = (int)m_VersionMap.GetMapping(entity->m_MappingHandle);

		for (auto & provider : m_Providers)
		{
			provider.second->Deallocate(mappedIndex);
		}

		m_VersionMap.RemoveMapping(entity->m_MappingHandle);
		m_Entities.erase(entity);
	}

	void Archetype::MoveEntity(EntityHandle & entity, Archetype & other)
	{
		int mappedIndex = (int)m_VersionMap.GetMapping(entity->m_MappingHandle);

		//TODO: Having to iterate over both collections here should be optimized into traversal of an ordered vector.
		//		With that method we can compare differences in layout in one iteration rather than two.
		for (auto & otherProvider : other.m_Providers)
		{
			ComponentProvider * theirPool = otherProvider.second;

			auto iter = m_Providers.find(otherProvider.first);

			if (iter == m_Providers.end())
			{
				theirPool->Allocate();
				continue;
			}

			ComponentProvider * myPool = iter->second;
			myPool->MoveEntityProvider(mappedIndex, *theirPool);
		}

		for (auto & myProvider : m_Providers)
		{
			ComponentProvider * myPool = myProvider.second;

			auto iter = other.m_Providers.find(myProvider.first);

			if (iter == other.m_Providers.end())
			{
				myProvider.second->Deallocate(mappedIndex);
				continue;
			}
		}

		m_VersionMap.RemoveMapping(entity->m_MappingHandle);
		entity->m_MappingHandle = other.m_VersionMap.AddMapping(other.GetEntityCount());
		entity->m_Archetype = &other;

		m_Entities.erase(entity);
		other.m_Entities.emplace(entity);
	}

	void Archetype::CloneAllProviders(const Archetype & other)
	{
		for (auto & pair : other.m_Providers)
		{
			m_Providers.emplace(pair.first, std::move(pair.second->CloneNew()));
		}
	}

	void Archetype::CloneAllProvidersExcept(const Archetype & other, const std::type_index & exception)
	{
		for (auto & pair : other.m_Providers)
		{
			if (pair.first != exception)
			{
				m_Providers.emplace(pair.first, std::move(pair.second->CloneNew()));
			}
		}
	}

	Archetype::Handle Archetype::GetHandle() const
	{
		Archetype::Handle handle;

		for (auto & map : m_Providers)
		{
			handle.AddComponent(map.first);
		}

		return handle;
	}

	std::size_t Archetype::GetEntityCount() const
	{
		return m_Entities.size();
	}

	std::size_t Archetype::GetProviderCount() const
	{
		return m_Providers.size();
	}

	const std::unordered_set<EntityHandle, EntityHandle::Hash> & Archetype::GetEntities() const
	{
		return m_Entities;
	}
}