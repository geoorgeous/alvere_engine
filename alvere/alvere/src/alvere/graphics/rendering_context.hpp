#pragma once

#include "alvere/graphics/frame_buffer.hpp"
#include "alvere/math/vector/vec_2_i.hpp"

namespace alvere
{
	class RenderingContext
	{
	public:

		virtual void swapBuffers() = 0;

		virtual void init(int width, int height) = 0;

		void bindFrameBuffer() const;

		Vec2i getResolution() const;

		float getAspectRatio() const;

		void setResolution(int width, int height);

	protected:

		std::unique_ptr<FrameBuffer> m_frameBuffer;
	};
}