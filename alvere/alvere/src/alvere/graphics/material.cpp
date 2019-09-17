#include <iostream>
#include <sstream>

#include "alvere/graphics/material.hpp"
#include "alvere/utils/utilities.hpp"

namespace alvere
{
	Material::Material(const ShaderProgram * shaderProgram)
		: m_shaderProgram(shaderProgram)
	{
		processShaderSource(shaderProgram->getShader(Shader::Type::Vertex));
		processShaderSource(shaderProgram->getShader(Shader::Type::Geometry));
		processShaderSource(shaderProgram->getShader(Shader::Type::Fragment));
	}

	void Material::processShaderSource(const Shader * shader)
	{
		if (shader == nullptr)
			return;

		std::istringstream buffer(shader->getSource());
		std::string line;

		while (std::getline(buffer, line))
		{
			if (line != "//property")
				continue;
			
			std::getline(buffer, line);

			utils::removeChar(line, ';');

			std::vector words = utils::splitString(line, ' ');

			if (words[0] != "uniform")
				continue;

			std::string uniformDataTypeStr = words[1];
			Shader::DataType uniformDataType;

			if (uniformDataTypeStr == "float") uniformDataType = Shader::DataType::Float;
			else if (uniformDataTypeStr == "vec2") uniformDataType = Shader::DataType::Float2;
			else if (uniformDataTypeStr == "vec3") uniformDataType = Shader::DataType::Float3;
			else if (uniformDataTypeStr == "vec4") uniformDataType = Shader::DataType::Float4;
			else if (uniformDataTypeStr == "int") uniformDataType = Shader::DataType::Int;
			else if (uniformDataTypeStr == "ivec2") uniformDataType = Shader::DataType::Int2;
			else if (uniformDataTypeStr == "ivec3") uniformDataType = Shader::DataType::Int3;
			else if (uniformDataTypeStr == "ivec4") uniformDataType = Shader::DataType::Int4;
			else if (uniformDataTypeStr == "mat4") uniformDataType = Shader::DataType::Mat4x4;
			else if (uniformDataTypeStr == "sampler2D") uniformDataType = Shader::DataType::Sampler2D;
			else continue;

			m_propertiesInfo.push_back(MaterialPropertyInfo{ uniformDataType, words[2] });
		}
	}
}