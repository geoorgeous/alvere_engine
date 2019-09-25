#pragma once

#include "alvere/world/scene_system.hpp"

namespace alvere
{
	class DrawableSceneSystem : public virtual SceneSystem
	{
	public:

		virtual void draw() = 0;
	};
}