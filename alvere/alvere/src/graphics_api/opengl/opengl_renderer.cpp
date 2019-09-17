#include <glad/glad.h>

#include "alvere/assets.hpp"
#include "alvere/graphics/mesh.hpp"
#include "alvere/utils/file_reader.hpp"
#include "graphics_api/opengl/opengl_renderer.hpp"
#include "graphics_api/opengl/opengl_errors.hpp"

namespace alvere::graphics_api::opengl
{
	Renderer::Renderer()
	{ }

	void Renderer::flush()
	{
		sortDrawCommands();

		for (const DrawCommand & command : m_drawCommands)
			processDrawCommand(command);

		m_drawCommands.clear();
	}

	void Renderer::processDrawCommand(const DrawCommand & command)
	{
		glEnable(GL_DEPTH_TEST);

		command.m_material->bind();

		command.m_material->getBaseMaterial()->getShaderProgram()->sendUniformMat4x4("u_projectionMatrix", *m_projectionMatrix);
		command.m_material->getBaseMaterial()->getShaderProgram()->sendUniformMat4x4("u_viewMatrix", *m_viewMatrix);
		command.m_material->getBaseMaterial()->getShaderProgram()->sendUniformMat4x4("u_modelMatrix", command.m_localTransform);

		glActiveTexture(GL_TEXTURE0);
		command.m_material->sendPropertiesToShader();

		m_VAO.AddVertexBuffer(command.m_mesh->GetVertexBuffer());
		m_VAO.SetIndexBuffer(command.m_mesh->GetElementBuffer());
		m_VAO.Bind();

		ALV_LOG_OPENGL_CALL(glDrawElements(GL_TRIANGLES, command.m_mesh->GetElementBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));

		command.m_material->unbind();
	}
}

alvere::Renderer* alvere::Renderer::New()
{
	return new alvere::graphics_api::opengl::Renderer();
}