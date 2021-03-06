#pragma once

#include <alvere/math/vectors.hpp>
#include <alvere/world/system/query_updated_system.hpp>
#include <alvere/world/component/components/c_sprite.hpp>

#include "components/rendering/c_spritesheet.hpp"

class S_Spritesheet : public alvere::QueryUpdatedSystem<const C_Spritesheet, alvere::C_Sprite>
{
public:

	void Update(float deltaTime, const C_Spritesheet & spritesheet, alvere::C_Sprite & sprite)
	{
		const alvere::RectI & textureSourceRect = spritesheet.m_SourceRect;
		
		alvere::RectI & spriteSourceRect = sprite.m_sprite.textureSource();
		spriteSourceRect = alvere::RectI(textureSourceRect.m_x + spritesheet.m_Offset[0] * textureSourceRect.m_width
									  , textureSourceRect.m_y + spritesheet.m_Offset[1] * textureSourceRect.m_height
									  , textureSourceRect.m_width
									  , textureSourceRect.m_height);
	}
};