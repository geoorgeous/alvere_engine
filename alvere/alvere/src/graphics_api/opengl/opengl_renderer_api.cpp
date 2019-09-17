#include <glad/glad.h>

#include "graphics_api/opengl/opengl_renderer_api.hpp"

namespace alvere::graphics_api::opengl
{
	void RendererAPI::SetClearColour(const alvere::Vector4& colour)
	{
		glClearColor(colour.x, colour.y, colour.z, colour.w);
	}

	void RendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}