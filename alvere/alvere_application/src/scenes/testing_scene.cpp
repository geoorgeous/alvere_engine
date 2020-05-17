
#include <alvere/world/scene/scene_loader.hpp>
#include <alvere\world\component\components\c_transform.hpp>
#include <alvere\world\component\components\c_mover.hpp>
#include <alvere\world\component\components\c_direction.hpp>

#include "testing_scene.hpp"

namespace alvere
{
	std::unique_ptr<Scene> TestingScene::LoadScene()
	{
		std::unique_ptr<Scene> scene = std::make_unique<Scene>();

		EntityHandle player = m_World.SpawnEntity<C_Transform, C_Mover>();
		m_World.GetComponent<C_Transform>(player)->setPosition(Vector3(2.0f, 0.0f, 0.0f));
		scene->AddEntity(player);

		return std::move( scene );
	}
}