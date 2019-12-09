#include "alvere/world/world.hpp"
#include "alvere/world/archetype/version_map.hpp"
#include "alvere/world/system/updated_system.hpp"
#include "alvere/world/system/rendered_system.hpp"

namespace alvere
{
	World::World()
	{
		m_Archetypes.emplace(Archetype::Handle::make_handle<>(), new Archetype());
	}

	World::~World()
	{
		for (auto iter : m_Archetypes)
		{
			delete iter.second;
		}

		for (auto iter : m_AllSystems)
		{
			delete iter.second;
		}
	}

	void World::Update(float deltaTime)
	{
		for (auto & system : m_UpdatedSystems)
		{
			system.second->Update(*this, deltaTime);
		}
	}

	void World::Render()
	{
		for (auto & system : m_RenderedSystems)
		{
			system.second->Render(*this);
		}
	}

	EntityHandle World::SpawnEntity()
	{
		EntityHandle e = m_Entities.allocate();

		m_Archetypes[Archetype::Handle::make_handle<>()]->AddEntity(e);

		return e;
	}

	void World::DestroyEntity(EntityHandle & entity)
	{
		Archetype * archetype = entity->m_Archetype;
		archetype->DestroyEntity(entity);

		m_Entities.deallocate(entity);
	}

	void World::QueryArchetypes(const Archetype::Query & query, std::vector<std::reference_wrapper<Archetype>> & matchingArchetypes) const
	{
		matchingArchetypes.clear();

		for (auto & archetype : m_Archetypes)
		{
			if (query.Matches(*archetype.second))
			{
				matchingArchetypes.emplace_back(*archetype.second);
			}
		}
	}
}