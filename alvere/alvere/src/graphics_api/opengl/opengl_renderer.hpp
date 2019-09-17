#pragma once

#include "alvere/graphics/renderer.hpp"

#include "opengl_shader_program.hpp"
#include "opengl_vertex_array.hpp"

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