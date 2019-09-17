#pragma once

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "alvere/utils/exceptions.hpp"
#include "alvere/utils/logging.hpp"
#include "graphics_api/opengl/opengl_context.hpp"

namespace alvere::graphics_api::opengl
{
	static bool s_GLADInitialised = false;

	RenderingContext::RenderingContext(GLFWwindow * windowHandle)
		: m_WindowHandle(windowHandle)
	{ }

	void RenderingContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

		if (!s_GLADInitialised && !gladLoaderLoadGL())
			AlvThrowFatal("Failed to initialise OpenGL context!");

		s_GLADInitialised = true;

		LogInfo("Successfully initialised OpenGL context:\n\tOpenGL %s\n\tGLSL %s\n\tDevice: %s\n",
			(const char *)glGetString(GL_VERSION),
			(const char *)glGetString(GL_SHADING_LANGUAGE_VERSION),
			(const char *)glGetString(GL_RENDERER));
	}

	void RenderingContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}