#include <glad/glad.h>
#include <vector>

#include "alvere/assets.hpp"
#include "alvere/graphics/shader_program.hpp"
#include "alvere/utils/logging.hpp"

namespace alvere
{
	const AssetRef<Shader> ShaderProgram::getShader(Shader::Type type) const
	{
		if (m_shaders.find(type) == m_shaders.end())
			return nullptr;
		return m_shaders.at(type);
	}

	void ShaderProgram::SetShader(const AssetRef<Shader> shader)
	{
		m_shaders[shader->getType()] = shader;
	}

	ShaderProgram::ShaderProgram()
		: m_handle(0)
	{ }
}