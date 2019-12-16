#include "graphics_api/opengl/opengl_shader.hpp"

#include <glad/glad.h>
#include <vector>

#include "alvere/debug/logging.hpp"

#include "graphics_api/opengl/opengl_errors.hpp"

namespace alvere::graphics_api::opengl
{
	Shader::Shader(Shader::Type type, const std::string& source)
		: alvere::Shader(type, source)
	{
		switch (type)
		{
		case Shader::Type::Vertex: m_Handle = glCreateShader(GL_VERTEX_SHADER); break;
		case Shader::Type::Geometry: m_Handle = glCreateShader(GL_GEOMETRY_SHADER); break;
		case Shader::Type::Fragment: m_Handle = glCreateShader(GL_FRAGMENT_SHADER); break;
		}
	}

	Shader::~Shader()
	{
		glDeleteShader(m_Handle);
	}

	bool Shader::Compile() const
	{
		if (m_IsCompiled)
			return true;

		const GLchar * source = m_Source.c_str();
		glShaderSource(m_Handle, 1, &source, 0);

		glCompileShader(m_Handle);

		GLint isCompiled = 0;
		glGetShaderiv(m_Handle, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(m_Handle, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(m_Handle, maxLength, &maxLength, &infoLog[0]);
			glDeleteShader(m_Handle);

			LogError("Failed to compile shader. OpenGL output: %s", infoLog.data());

			return false;
		}

		m_IsCompiled = true;

		return true;
	}
}

std::unique_ptr<alvere::Shader> alvere::Shader::New(Shader::Type type, const std::string& source = "")
{
	return std::make_unique<alvere::graphics_api::opengl::Shader>(type, source);
}