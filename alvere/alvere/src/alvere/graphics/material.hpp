#pragma once

#include <vector>

#include "alvere/graphics/shader_program.hpp"

namespace alvere
{
	struct MaterialPropertyInfo
	{
		Shader::DataType m_shaderDataType;
		std::string m_id;
	};

	class Material
	{
	public:

		Material(const ShaderProgram * shaderProgram);

		inline const ShaderProgram * getShaderProgram() const
		{
			return m_shaderProgram;
		}

		inline const std::vector<MaterialPropertyInfo> & getPropertiesInfo() const
		{
			return m_propertiesInfo;
		}

	private:

		const ShaderProgram * m_shaderProgram;

		std::vector<MaterialPropertyInfo> m_propertiesInfo;

		void processShaderSource(const Shader * shader);
	};
}