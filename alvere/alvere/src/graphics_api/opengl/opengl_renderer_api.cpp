#include <glad/glad.h>

#include "graphics_api/opengl/opengl_renderer_api.hpp"

namespace alvere::graphics_api::opengl
{
	void RendererAPI::setClearColour(const alvere::Vector4& colour)
	{
		glClearColor(colour.x, colour.y, colour.z, colour.w);
	}

	void RendererAPI::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RendererAPI::setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	{
		glViewport((GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height);
	}
}