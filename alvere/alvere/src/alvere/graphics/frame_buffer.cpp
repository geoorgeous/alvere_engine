#include "alvere/graphics/frame_buffer.hpp"

namespace alvere
{
	FrameBuffer::FrameBuffer()
		: m_width(0), m_height(0), m_aspect(0.0f)
	{}

	FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
		: m_width(width), m_height(height), m_aspect((float)height / width)
	{}

	FrameBuffer::~FrameBuffer()
	{}
}