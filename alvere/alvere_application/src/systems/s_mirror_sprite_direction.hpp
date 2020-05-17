#pragma once

#include <alvere/math/vectors.hpp>
#include <alvere/world/system/query_updated_system.hpp>
#include <alvere/world/component/components/c_sprite.hpp>

#include "components/c_direction.hpp"

class S_MirrorSpriteDirection : public alvere::QueryUpdatedSystem<const C_Direction, alvere::C_Sprite>
{
public:

	void Update(float deltaTime, const C_Direction & direction, alvere::C_Sprite & sprite)
	{
		sprite.m_mirrored[0] = direction.m_Direction == Direction::Left;
	}
};