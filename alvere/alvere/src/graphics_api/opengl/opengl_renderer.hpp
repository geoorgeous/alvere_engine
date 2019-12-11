#pragma once

#include "alvere/graphics/renderer.hpp"

#include "graphics_api/opengl/opengl_shader_program.hpp"
#include "graphics_api/opengl/opengl_vertex_array.hpp"

namespace alvere::graphics_api::opengl
{
	class Renderer : public alvere::Renderer
	{
	public:

		Renderer();

	private:

		VertexArray m_VAO;

		void flush() override;

		void processDrawCommand(const DrawCommand & command) override;
	};
}