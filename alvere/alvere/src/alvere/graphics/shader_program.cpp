#include "alvere/graphics/shader_program.hpp"

#include <glad/glad.h>
#include <vector>

namespace alvere
{
	const Shader * ShaderProgram::getShader(Shader::Type type) const
	{
		if (m_shaders.find(type) == m_shaders.end())
			return nullptr;
		return m_shaders.at(type);
	}

	void ShaderProgram::SetShader(const Shader * shader)
	{
		m_shaders[shader->getType()] = shader;
	}

	ShaderProgram::ShaderProgram()
		: m_handle(0)
	{ }
}