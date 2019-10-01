#pragma once

#include "alvere/graphics/rendering_context.hpp"

struct GLFWwindow;

namespace alvere::graphics_api::opengl
{
	class RenderingContext : public alvere::RenderingContext
	{
	public:
		RenderingContext(GLFWwindow* windowHandle);

		void init() override;
		void swapBuffers() override;

	private:
		GLFWwindow * m_windowHandle;
	};

	int static LogOpenGLErrors();

	bool static LogLastOpenGLError();
}