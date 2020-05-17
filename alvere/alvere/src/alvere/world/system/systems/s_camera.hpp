#pragma once

#include "alvere/world/system/query_updated_system.hpp"
#include "alvere\world\component\components\c_camera.hpp"
#include "alvere\world\component\components\c_transform.hpp"

namespace alvere
{
	class S_Camera : public QueryUpdatedSystem<const C_Transform, C_Camera>
	{
	public:

		//Since the Camera class is a standalone class, the transform position needs to be pushed into it
		void Update(float deltaTime, const C_Transform & transform, C_Camera & camera)
		{
			camera.setPosition(transform->getPosition());
		}
	};
}
