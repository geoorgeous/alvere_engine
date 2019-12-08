#pragma once

namespace alvere
{
	class RenderingContext
	{
	public:

		virtual void init() = 0;

		virtual void bindFrameBuffer() = 0;

		virtual void swapBuffers() = 0;
	};
}