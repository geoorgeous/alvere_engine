#pragma once

#include <string>

#include <alvere/world/world.hpp>
#include <alvere/world/scene/scene_loader.hpp>
#include <alvere/world/entity/entity_handle.hpp>

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

	template <typename T>
	alvere::EntityHandle SpawnFromDefinition(alvere::Scene & scene);

	void SpawnMap(std::unique_ptr<alvere::Scene> & scene);
	bool LoadMap(std::unique_ptr<alvere::Scene> & scene, const std::string & filepath);
};

template <typename T>
alvere::EntityHandle PlatformerScene::SpawnFromDefinition(alvere::Scene & scene)
{
	T definition;
	alvere::EntityHandle entity = definition.SpawnInstance(m_World);
	scene.AddEntity(entity);
	return entity;
}