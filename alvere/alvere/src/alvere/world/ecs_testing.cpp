#include <cassert>
#include <iostream>

#include "alvere/world/world.hpp"
#include "alvere/world/entity/entity.hpp"

#include "alvere/world/application/c_transform.hpp"
#include "alvere/world/application/c_mover.hpp"
#include "alvere/world/application/c_direction.hpp"
#include "alvere/world/application/c_saveable.hpp"
#include "alvere/world/application/mover_system.hpp"

namespace alvere
{
	void UpdateTests()
	{
		World world;

		{
			Entity player = world.SpawnEntity<C_Transform, C_Mover>();

			world.GetComponent<C_Transform>(player).m_X = 2;
		}

		{
			Entity player = world.SpawnEntity();
			world.AddComponent<C_Transform>(player);
			world.AddComponent<C_Mover>(player);

			world.GetComponent<C_Transform>(player).m_X = 3;
		}

		{
			Entity player = world.SpawnEntity();
			world.AddComponent<C_Transform>(player);
			world.AddComponent<C_Mover>(player);
			world.AddComponent<Direction>(player);

			world.GetComponent<C_Transform>(player).m_X = 5;
		}

		{
			Entity player = world.SpawnEntity();
			world.AddComponent<C_Transform>(player);

			world.GetComponent<C_Transform>(player).m_X = 7;
		}

		world.AddSystem<MoverSystem>();

		world.Update(1.0f);
		world.Render();

		world.RemoveSystem<MoverSystem>();

		world.Update(1.0f);
		world.Render();
	}

	void ComponentTests()
	{
		World world;

		Entity player = world.SpawnEntity();
		assert(player.m_Archetype->GetProviderCount() == 0);

		world.AddComponent<C_Transform>(player);
		assert(player.m_Archetype->GetProviderCount() == 1);

		world.AddComponent<C_Saveable>(player);
		assert(player.m_Archetype->GetProviderCount() == 2);

		{
			C_Transform & transform = world.GetComponent<C_Transform>(player);
			transform.m_X = 5;
		}

		world.AddComponent<C_Mover>(player);
		assert(player.m_Archetype->GetProviderCount() == 3);

		{
			C_Transform & transform = world.GetComponent<C_Transform>(player);
			assert(transform.m_X == 5);

			C_Mover & mover = world.GetComponent<C_Mover>(player);
			mover.m_Speed = 2;
		}

		world.RemoveComponent<C_Transform>(player);
		assert(player.m_Archetype->GetProviderCount() == 2);

		{
			C_Mover & mover = world.GetComponent<C_Mover>(player);
			assert(mover.m_Speed == 2);
		}

		Entity player3 = world.SpawnEntity<C_Transform, C_Saveable, C_Mover>();
		assert(player3.m_Archetype->GetProviderCount() == 3);
		world.GetComponent<C_Transform>(player3).m_X = 5;

		Entity player2 = world.SpawnEntity();
		assert(player2.m_Archetype->GetProviderCount() == 0);

		world.AddComponent<C_Mover>(player2);
		assert(player2.m_Archetype->GetProviderCount() == 1);

		{
			C_Mover & mover = world.GetComponent<C_Mover>(player2);
			mover.m_Speed = 10;
		}

		world.AddComponent<C_Saveable>(player2);
		assert(player2.m_Archetype->GetProviderCount() == 2);

		{
			C_Mover & mover = world.GetComponent<C_Mover>(player2);
			assert(mover.m_Speed == 10);
			mover.m_Speed = 11;
		}

		world.DestroyEntity(player);

		{
			C_Mover & mover = world.GetComponent<C_Mover>(player2);
			assert(mover.m_Speed == 11);
		}

		world.DestroyEntity(player2);

		assert(world.GetComponent<C_Transform>(player3).m_X == 5);

		world.DestroyEntity(player3);
	}

	void RunTests()
	{
		UpdateTests();
		ComponentTests();
	}
}