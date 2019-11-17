#pragma once

#include <alvere/world/scene/scene_loader.hpp>

namespace alvere
{
	class TestingScene : public SceneLoader
	{
		World & m_World;

	public:

		TestingScene(World & world)
			: m_World(world)
		{
		}

		virtual std::unique_ptr<Scene> LoadScene() override;
	};
}