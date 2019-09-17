#pragma once

#include "alvere/graphics/rendering_context.hpp"

struct GLFWwindow;

namespace alvere::graphics_api::opengl
{
	class RenderingContext : public alvere::RenderingContext
	{
	public:
		RenderingContext(GLFWwindow* windowHandle);

		void Init() override;
		void SwapBuffers() override;

	private:
		GLFWwindow * m_WindowHandle;
	};

	int static LogOpenGLErrors();

	bool static LogLastOpenGLError();
}