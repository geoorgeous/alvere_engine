#pragma once

#include <alvere/math/vectors.hpp>
#include <alvere/utils/shapes.hpp>
#include <alvere/world/component/pooled_component.hpp>

class C_Spritesheet : public alvere::PooledComponent<C_Spritesheet>
{
public:

	alvere::RectI m_SourceRect;
	alvere::Vector2i m_Offset;
};