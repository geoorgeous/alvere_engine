#pragma once

#include <unordered_map>

#include "alvere/graphics/renderer.hpp"
#include "alvere/world/entity_component_system.hpp"

namespace alvere
{
	class ECSSceneRenderer : public SceneSystem
	{
	public:

		ECSSceneRenderer();

		~ECSSceneRenderer();

		void updateSystems(EntityComponentMap & entityComponentMap, float timeStep) override;

	private:

		Renderer * m_renderer;
	};
}