#pragma once

#include <map>

#include "alvere/graphics/text/font.hpp"
#include "alvere/math/vector/vector_2.hpp"

namespace alvere
{
	class TextRenderer
	{
	public:

		void drawText(const AssetRef<Font> font, std::string text, Vector2 position, unsigned int size, Vector4 colour);
	};
}