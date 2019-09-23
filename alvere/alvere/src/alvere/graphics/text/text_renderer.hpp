#pragma once

#include <map>

#include "alvere/graphics/text/font.hpp"
#include "alvere/math/vector/vector_2.hpp"

namespace alvere
{
	class TextRenderer
	{
	public:

		void drawText(SpriteBatcher & spriteBatcher, const AssetRef<Font::Face> fontface, std::string text, Vector2 position, float lineHeight, Vector4 colour);
	};
}