#pragma once

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "graphics_api/opengl/opengl_context.hpp"

#include "alvere/graphics/shader.hpp"
#include "alvere/utils/exceptions.hpp"
#include "alvere/utils/logging.hpp"

namespace alvere::graphics_api::opengl
{
	static bool s_GLADInitialised = false;

	RenderingContext::RenderingContext(GLFWwindow * windowHandle)
		: m_windowHandle(windowHandle)
	{ }

	RenderingContext::~RenderingContext()
	{
		glDeleteVertexArrays(1, &m_screenQuadVAOHandle);
		glDeleteBuffers(1, &m_screenQuadVBOHandle);
	}

	void RenderingContext::init()
	{
		glfwMakeContextCurrent(m_windowHandle);

		if (!s_GLADInitialised && !gladLoaderLoadGL())
			AlvThrowFatal("Failed to initialise OpenGL context!");

		s_GLADInitialised = true;

		m_frameBuffer = FrameBuffer::create(1024, 768);

		LogInfo("Successfully initialised OpenGL context:\n\tOpenGL %s\n\tGLSL %s\n\tDevice: %s\n",
			(const char *)glGetString(GL_VERSION),
			(const char *)glGetString(GL_SHADING_LANGUAGE_VERSION),
			(const char *)glGetString(GL_RENDERER));

		// Screen quad stuff

		std::unique_ptr<Shader> screenQuadShaderV = Shader::New(Shader::Type::Vertex, R"(
			#version 330 core
			layout (location = 0) in vec2 a_Position;
			layout (location = 1) in vec2 a_TexCoords;

			out vec2 v_TexCoords;

			void main()
			{
				gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0); 
				v_TexCoords = a_TexCoords;
			})");
		std::unique_ptr<Shader> screenQuadShaderF = Shader::New(Shader::Type::Fragment, R"(
			#version 330 core

			uniform sampler2D u_ScreenTexture;
  
			in vec2 v_TexCoords;

			out vec4 f_FragColour;

			void main()
			{ 
				f_FragColour = texture(u_ScreenTexture, v_TexCoords);
			})");

		m_screenQuadShaderProgram = ShaderProgram::New();
		m_screenQuadShaderProgram->SetShader(screenQuadShaderV.get());
		m_screenQuadShaderProgram->SetShader(screenQuadShaderF.get());
		m_screenQuadShaderProgram->build();

		float screenQuadVertices[] = {
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		glGenVertexArrays(1, &m_screenQuadVAOHandle);
		glBindVertexArray(m_screenQuadVAOHandle);

		glGenBuffers(1, &m_screenQuadVBOHandle);
		glBindBuffer(GL_ARRAY_BUFFER, m_screenQuadVBOHandle);

		glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertices), &screenQuadVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
	}

	void RenderingContext::bindFrameBuffer()
	{
		m_frameBuffer->bind();
	}

	void RenderingContext::swapBuffers()
	{
		m_frameBuffer->unbind(); // unbinding custom framebuffer/binding the default frame buffer

		drawFrameBuffer(); // rendering the custom frame buffer to the default framebufer and screen quad

		glfwSwapBuffers(m_windowHandle);
	}

	void RenderingContext::drawFrameBuffer()
	{
		// bind the shader that draws the screen quad
		m_screenQuadShaderProgram->bind();

		// send framebuffer texture to shader
		glActiveTexture(GL_TEXTURE0);
		m_frameBuffer->getTexture().bind();

		// bind the vao with the screen quad
		glBindVertexArray(m_screenQuadVAOHandle);
		
		// draw the screen quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		m_frameBuffer->getTexture().unbind();

		m_screenQuadShaderProgram->unbind();
	}
}