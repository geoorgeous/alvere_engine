#pragma once

#include "alvere/graphics/renderer.hpp"
#include "alvere/world/entity_component.hpp"
#include "alvere/world/entity_component_system.hpp"

namespace alvere
{
	class ECSSceneRenderer : public EntityComponentSystem
	{
	public:

		ECSSceneRenderer();

		~ECSSceneRenderer();

		void update(std::unordered_map<std::type_index, std::pair<unsigned int, EntityComponent *>> & components, float timeStep) override;

	private:

		Renderer * m_renderer;
	};
}