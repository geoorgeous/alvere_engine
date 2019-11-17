#pragma once

#include <alvere/world/world.hpp>
#include <alvere/world/scene/scene_loader.hpp>

class PlatformerScene : public alvere::SceneLoader
{
	alvere::World & m_World;

public:

	PlatformerScene(alvere::World & world)
		: m_World(world)
	{
	}

	virtual std::unique_ptr<alvere::Scene> LoadScene() override;

private:

	void SpawnPlayer(std::unique_ptr<alvere::Scene>& scene);
};