#pragma once

#include "alvere/graphics/buffers.hpp"
#include "alvere/graphics/frame_buffer.hpp"
#include "alvere/graphics/rendering_context.hpp"
#include "alvere/graphics/shader_program.hpp"
#include "graphics_api/opengl/opengl_vertex_array.hpp"

struct GLFWwindow;

namespace alvere::graphics_api::opengl
{
	class RenderingContext : public alvere::RenderingContext
	{
	public:

		RenderingContext();

		~RenderingContext();

		void init(int width, int height) override;

		void renderFrameBuffer() override;

	private:
		
		std::unique_ptr<ShaderProgram> m_screenQuadShaderProgram;

		unsigned int m_screenQuadVAOHandle;
		unsigned int m_screenQuadVBOHandle;
	};

	int static LogOpenGLErrors();

	bool static LogLastOpenGLError();
}