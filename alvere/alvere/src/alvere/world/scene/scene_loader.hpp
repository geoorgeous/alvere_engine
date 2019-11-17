#pragma once

#include <memory>

#include "alvere/world/world.hpp"
#include "alvere/world/scene/scene.hpp"

namespace alvere
{
	class SceneLoader
	{
	public:

		virtual std::unique_ptr<Scene> LoadScene() = 0;
	};
}