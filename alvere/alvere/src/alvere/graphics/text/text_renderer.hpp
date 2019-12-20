#pragma once

#include <string>

#include "alvere/graphics/text/font.hpp"
#include "alvere/math/matrix/matrix_4.hpp"

namespace alvere
{
	class TextRenderer
	{
	public:

		void submit(const std::string & text, const Font::Face::Bitmap * fontFaceBitmap, Matrix4 transformMatrix);
	};
}