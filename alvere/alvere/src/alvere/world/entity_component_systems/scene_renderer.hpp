#pragma once

#include <unordered_map>

#include "alvere/graphics/renderer.hpp"
#include "alvere/world/drawable_scene_system.hpp"
#include "alvere/world/updated_scene_system.hpp"

namespace alvere
{
	class SceneRenderer : public DrawableSceneSystem
	{
	public:

		void draw() override;

	private:
	};
}