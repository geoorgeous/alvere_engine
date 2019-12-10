#pragma once

#include "alvere/graphics/frame_buffer.hpp"

namespace alvere::graphics_api::opengl
{
	class FrameBuffer : public ::alvere::FrameBuffer
	{
	public:

		FrameBuffer();

		FrameBuffer(unsigned int width, unsigned int height);

		FrameBuffer(const FrameBuffer& frameBuffer);

		~FrameBuffer() override;

		FrameBuffer& operator=(const FrameBuffer& frameBuffer);

		void resize(int width, int height) override;

		void bind() const override;

		void unbind() const override;

	private:

		unsigned int m_handle;
		unsigned int m_depthStencilBufferHandle;

		void init(unsigned int width, unsigned int height);
	};
}