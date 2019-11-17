#include "alvere/world/scene/scene_system.hpp"

namespace alvere
{
	SceneSystem::SceneSystem(World & world)
		: m_World(world)
	{
	}

	void SceneSystem::Update(World & world, float deltaTime)
	{
	}

	Scene & SceneSystem::LoadScene(SceneLoader & sceneLoader)
	{
		std::unique_ptr<Scene> scene = sceneLoader.LoadScene();

		m_Scenes.push_back(std::move(scene));

		return *m_Scenes.back();
	}

	void SceneSystem::UnloadScene(Scene & scene)
	{
		auto iter = FindScene(scene);

		AlvAssert(iter != m_Scenes.end(), "Must pass an already loaded scene to unload");

		for (EntityHandle & entity : scene.m_Entities)
		{
			m_World.DestroyEntity(entity);
		}

		m_Scenes.erase(iter);
	}

	std::vector<std::unique_ptr<Scene>>::iterator SceneSystem::FindScene(Scene & scene)
	{
		for (int i = 0; i < m_Scenes.size(); ++i)
		{
			if (&scene == m_Scenes[i].get())
			{
				return m_Scenes.begin() + i;
			}
		}

		return m_Scenes.end();
	}
}