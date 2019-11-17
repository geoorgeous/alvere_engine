#pragma once

#include <vector>
#include <algorithm>
#include <memory>

#include "alvere/world/scene/scene_loader.hpp"
#include "alvere/world/scene/scene.hpp"
#include "alvere/world/system/updated_system.hpp"
#include "alvere/utils/exceptions.hpp"

namespace alvere
{
	class SceneSystem : public UpdatedSystem
	{
		World & m_World;

		std::vector<std::unique_ptr<Scene>> m_Scenes;

	public:

		SceneSystem(World & world);

		virtual void Update(World & world, float deltaTime) override;

		Scene & LoadScene(SceneLoader & sceneLoader);

		void UnloadScene(Scene & scene);

	private:

		std::vector<std::unique_ptr<Scene>>::iterator FindScene(Scene & scene);
	};
}