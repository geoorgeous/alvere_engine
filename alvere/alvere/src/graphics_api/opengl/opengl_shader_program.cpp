#pragma once

#include <glad/glad.h>

#include "alvere/utils/logging.hpp"

#include "graphics_api/opengl/opengl_shader_program.hpp"

namespace alvere::graphics_api::opengl
{
	ShaderProgram::ShaderProgram()
		: alvere::ShaderProgram()
	{
		m_handle = glCreateProgram();
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_handle);
	}

	bool ShaderProgram::build()
	{
		for (auto const & shader : m_shaders)
		{
			if (!shader.second->Compile())
				return false;
			glAttachShader(m_handle, shader.second->GetHandle());
		}

		// Link our program
		glLinkProgram(m_handle);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_handle, GL_LINK_STATUS, (int*)&isLinked);

		if (isLinked == GL_TRUE)
			return true;

		GLint maxLength = 0;
		glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_handle, maxLength, &maxLength, &infoLog[0]);

		LogError("Failed to compile shader program. OpenGL output: %s", infoLog.data());

		return false;
	}

	void ShaderProgram::bind() const
	{
		glUseProgram(m_handle);
	}

	void ShaderProgram::unbind() const
	{
		glUseProgram(0);
	}

	void ShaderProgram::sendUniformInt1(const std::string & id, int value) const
	{
		int loc = GetUniformLocation(id);
		glUniform1i(loc, value);
	}

	void ShaderProgram::sendUniformInt2(const std::string & id, int value1, int value2) const
	{
		int loc = GetUniformLocation(id);
		glUniform2i(loc, value1, value2);
	}

	void ShaderProgram::sendUniformInt3(const std::string & id, int value1, int value2, int value3) const
	{
		int loc = GetUniformLocation(id);
		glUniform3i(loc, value1, value2, value3);
	}

	void ShaderProgram::sendUniformInt4(const std::string & id, int value1, int value2, int value3, int value4) const
	{
		int loc = GetUniformLocation(id);
		glUniform4i(loc, value1, value2, value3, value4);
	}

	void ShaderProgram::sendUniformFloat1(const std::string & id, float value) const
	{
		int loc = GetUniformLocation(id);
		glUniform1f(loc, value);
	}

	void ShaderProgram::sendUniformFloat2(const std::string & id, float value1, float value2) const
	{
		int loc = GetUniformLocation(id);
		glUniform2f(loc, value1, value2);
	}

	void ShaderProgram::sendUniformFloat3(const std::string & id, float value1, float value2, float value3) const
	{
		int loc = GetUniformLocation(id);
		glUniform3f(loc, value1, value2, value3);
	}

	void ShaderProgram::sendUniformFloat4(const std::string & id, float value1, float value2, float value3, float value4) const
	{
		int loc = GetUniformLocation(id);
		glUniform4f(loc, value1, value2, value3, value4);
	}

	void ShaderProgram::sendUniformMat4x4(const std::string & id, const alvere::Matrix4 & matrix) const
	{
		int loc = GetUniformLocation(id);
		glUniformMatrix4fv(loc, 1, false, &matrix[0][0]);
	}

	int ShaderProgram::GetUniformLocation(const std::string & id) const
	{
		return glGetUniformLocation(m_handle, id.c_str());
	}
}

alvere::Asset<alvere::ShaderProgram> alvere::ShaderProgram::New()
{
	return alvere::Asset<ShaderProgram>(new alvere::graphics_api::opengl::ShaderProgram());
}