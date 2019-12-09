#pragma once

#include "alvere/graphics/frame_buffer.hpp"

namespace alvere
{
	class RenderingContext
	{
	public:

		virtual void swapBuffers() = 0;

		virtual void init(unsigned int width, unsigned int height) = 0;

		inline FrameBuffer * frameBuffer()
		{
			return m_frameBuffer.get();
		}

	protected:

		std::unique_ptr<FrameBuffer> m_frameBuffer;
	};
}