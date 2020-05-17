#pragma once

#include <alvere/math/vectors.hpp>
#include <alvere/world/system/query_updated_system.hpp>
#include <alvere/world/component/components/c_sprite.hpp>

#include "components/rendering/c_animation.hpp"
#include "components/rendering/c_spritesheet.hpp"

class S_Animation : public alvere::QueryUpdatedSystem<C_Animation, C_Spritesheet>
{
public:

	void Update(float deltaTime, C_Animation & animation, C_Spritesheet & sprite)
	{
		if (animation.m_CurrentAnimation == -1)
		{
			return;
		}

		const std::vector<C_Animation::Frame> & frames = animation.CurrentAnimation().m_Frames;

		if (animation.m_CurrentRuntime < frames[animation.m_CurrentFrame].m_Duration)
		{
			animation.m_CurrentRuntime += deltaTime;
			return;
		}

		if (animation.m_CurrentFrame != frames.size() - 1)
		{
			++animation.m_CurrentFrame;
			animation.m_CurrentRuntime = 0.0f;
		}
		else if (animation.CurrentAnimation().m_Loop)
		{
			animation.m_CurrentFrame = 0;
			animation.m_CurrentRuntime = 0.0f;
		}
		else
		{
			animation.m_CurrentAnimation = -1;
		}

		//Check we didn't just finish playing the animation
		if (animation.m_CurrentAnimation != -1)
		{
			sprite.m_Offset = frames[animation.m_CurrentFrame].m_SpriteOffset;
		}
	}
};