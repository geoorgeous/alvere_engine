#include "alvere/graphics/rendering_context.hpp"

#include "alvere/graphics/render_commands.hpp"

namespace alvere
{
	void RenderingContext::bindFrameBuffer() const
	{
		m_frameBuffer->bind();

		render_commands::setViewport(0, 0, m_frameBuffer->getSize().x, m_frameBuffer->getSize().y);

		render_commands::clear();
	}

	Vec2i RenderingContext::getResolution() const
	{
		return m_frameBuffer->getSize();
	}

	float RenderingContext::getAspectRatio() const
	{
		return m_frameBuffer->getAspect();
	}

	void RenderingContext::setResolution(int width, int height)
	{
		m_frameBuffer->resize(width, height);
	}
}