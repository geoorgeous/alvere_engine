#include <cassert>
#include <iostream>

#include "alvere/world/world.hpp"
#include "alvere/world/entity/entity.hpp"

#include "alvere/world/application/c_mover.hpp"
#include "alvere/world/application/c_direction.hpp"
#include "alvere/world/application/mover_system.hpp"
#include "alvere/world/component/components/c_transform.hpp"
#include "alvere/world/component/components/c_saveable.hpp"
#include <alvere\world\component\components\c_destroy.hpp>
#include <alvere\world\system\systems\destroy_system.hpp>

namespace alvere
{
	void UpdateTests()
	{
		World world;

		{
			EntityHandle player = world.SpawnEntity<C_Transform, C_Mover>();

			world.GetComponent<C_Transform>(player)->setPosition(Vector3(2.0f, 0.0f, 0.0f));
		}

		{
			EntityHandle player = world.SpawnEntity();
			world.AddComponent<C_Transform>(player);
			world.AddComponent<C_Mover>(player);

			world.GetComponent<C_Transform>(player)->setPosition(Vector3(3.0f, 0.0f, 0.0f));
		}

		{
			EntityHandle player = world.SpawnEntity();
			world.AddComponent<C_Transform>(player);
			world.AddComponent<C_Mover>(player);
			world.AddComponent<C_Direction>(player);

			world.GetComponent<C_Transform>(player)->setPosition(Vector3(5.0f, 0.0f, 0.0f));
		}

		{
			EntityHandle player = world.SpawnEntity();
			world.AddComponent<C_Transform>(player);

			world.GetComponent<C_Transform>(player)->setPosition(Vector3(7.0f, 0.0f, 0.0f));
		}

		world.AddSystem<MoverSystem>();
		MoverSystem* moverSystem = world.GetSystem<MoverSystem>();
		assert( moverSystem != nullptr );

		world.Update(1.0f);
		world.Render();

		world.RemoveSystem<MoverSystem>();

		world.Update(1.0f);
		world.Render();
	}

	void ComponentTests()
	{
		World world;

		EntityHandle player = world.SpawnEntity();
		assert(player->m_Archetype->GetProviderCount() == 0);

		world.AddComponent<C_Transform>(player);
		assert(player->m_Archetype->GetProviderCount() == 1);

		world.AddComponent<C_Saveable>(player);
		assert(player->m_Archetype->GetProviderCount() == 2);

		{
			world.GetComponent<C_Transform>(player)->setPosition(Vector3(5.0f, 0.0f, 0.0f));
		}

		world.AddComponent<C_Mover>(player);
		assert(player->m_Archetype->GetProviderCount() == 3);

		{
			C_Transform & transform = world.GetComponent<C_Transform>(player);
			assert(transform->getPosition().x == 5);

			C_Mover & mover = world.GetComponent<C_Mover>(player);
			mover.m_Speed = 2;
		}

		world.RemoveComponent<C_Transform>(player);
		assert(player->m_Archetype->GetProviderCount() == 2);

		{
			C_Mover & mover = world.GetComponent<C_Mover>(player);
			assert(mover.m_Speed == 2);
		}

		EntityHandle player3 = world.SpawnEntity<C_Transform, C_Saveable, C_Mover>();
		assert(player3->m_Archetype->GetProviderCount() == 3);
		world.GetComponent<C_Transform>(player3)->setPosition(Vector3(5.0f, 0.0f, 0.0f));

		EntityHandle player2 = world.SpawnEntity();
		assert(player2->m_Archetype->GetProviderCount() == 0);

		world.AddComponent<C_Mover>(player2);
		assert(player2->m_Archetype->GetProviderCount() == 1);

		{
			C_Mover & mover = world.GetComponent<C_Mover>(player2);
			mover.m_Speed = 10;
		}

		world.AddComponent<C_Saveable>(player2);
		assert(player2->m_Archetype->GetProviderCount() == 2);

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

		assert(world.GetComponent<C_Transform>(player3)->getPosition().x == 5);

		world.DestroyEntity(player3);
	}

	void DestroyTest()
	{
		World world;

		world.AddSystem<DestroySystem>();

		EntityHandle entity1 = world.SpawnEntity<C_Transform>();
		EntityHandle entity1_copy = entity1;
		assert(entity1->m_Archetype->GetEntityCount() == 1);

		EntityHandle entity2 = world.SpawnEntity<C_Transform>();
		assert(entity2->m_Archetype->GetEntityCount() == 2);

		world.Update(1.0f);

		assert(entity2->m_Archetype->GetEntityCount() == 2);

		world.AddComponent<C_Destroy>(entity1);

		assert(entity1.isValid());
		assert(entity1_copy.isValid());

		world.Update(1.0f);

		assert(entity1.isValid() == false);
		assert(entity1_copy.isValid() == false);
		assert(entity2->m_Archetype->GetEntityCount() == 1);

		world.AddComponent<C_Destroy>(entity2);

		assert(entity2.isValid());

		world.Update(1.0f);

		assert(entity2.isValid() == false);
	}

	void RunTests()
	{
		UpdateTests();
		ComponentTests();
		DestroyTest();
	}
}