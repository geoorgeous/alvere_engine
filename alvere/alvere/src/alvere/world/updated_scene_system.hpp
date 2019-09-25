#pragma once

#include "alvere/world/scene_system.hpp"

namespace alvere
{
	class UpdatedSceneSystem : public virtual SceneSystem
	{
	public:

		virtual void update(float deltaTime) = 0;
	};
}