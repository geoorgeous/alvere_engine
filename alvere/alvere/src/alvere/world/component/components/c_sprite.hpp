#pragma once

#include "alvere/graphics/texture.hpp"
#include "alvere/graphics/sprite.hpp"
#include "alvere/math/vector/vector_4.hpp"
#include "alvere/world/component/pooled_component.hpp"

namespace alvere
{
	class C_Sprite : public PooledComponent<C_Sprite>
	{
	public:

		Sprite m_sprite;
		alvere::Vector<bool, 2> m_mirrored;
	};
}