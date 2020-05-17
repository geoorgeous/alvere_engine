#include "s_destroy.hpp"

#include "alvere/world/component/components/c_destroy.hpp"

namespace alvere
{
	S_Destroy::S_Destroy()
		: m_DestroyQuery(Archetype::Query().Include<C_Destroy>())
	{
	}

	void S_Destroy::Update(World & world, float deltaTime)
	{
		world.QueryArchetypes(m_DestroyQuery, m_Archetypes);

		for (Archetype & archetype : m_Archetypes)
		{
			while (archetype.GetEntityCount() > 0)
			{
				EntityHandle entityHandle = *archetype.GetEntities().begin();
				world.DestroyEntity(entityHandle);
			}
		}
	}
}