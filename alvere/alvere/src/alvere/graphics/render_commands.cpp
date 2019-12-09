#include "alvere/graphics/render_commands.hpp"
#include "alvere/graphics/renderer_api.hpp"

namespace alvere::render_commands
{
	void setClearColour(const alvere::Vector4& colour)
	{
		RendererAPI::s_Instance->setClearColour(colour);
	}

	void clear()
	{
		RendererAPI::s_Instance->clear();
	}

	void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	{
		RendererAPI::s_Instance->setViewport(x, y, width, height);
	}
}