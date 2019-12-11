#include "alvere/graphics/shader.hpp"

#include <glad/glad.h>
#include <vector>

#include "alvere/debug/logging.hpp"

namespace alvere
{
	unsigned int Shader::GetDataTypeSize(Shader::DataType dataType)
	{
		switch(dataType)
		{
			case DataType::Float:		return sizeof(float) * 1;
			case DataType::Float2:		return sizeof(float) * 2;
			case DataType::Float3:		return sizeof(float) * 3;
			case DataType::Float4:		return sizeof(float) * 4;
			case DataType::Int:			return sizeof(int) * 1;
			case DataType::Int2:		return sizeof(int) * 2;
			case DataType::Int3:		return sizeof(int) * 3;
			case DataType::Int4:		return sizeof(int) * 4;
			case DataType::Mat4x4:		return sizeof(float) * 16;
			case DataType::Sampler2D:	return sizeof(int) * 1;
		}
		return 0;
	}

	unsigned int Shader::GetDataTypeComponentCount(Shader::DataType dataType)
	{
		switch(dataType)
		{
			case DataType::Float:		return 1;
			case DataType::Float2:		return 2;
			case DataType::Float3:		return 3;
			case DataType::Float4:		return 4;
			case DataType::Int:			return 1;
			case DataType::Int2:		return 2;
			case DataType::Int3:		return 3;
			case DataType::Int4:		return 4;
			case DataType::Mat4x4:		return 16;
			case DataType::Sampler2D:	return 1;
		}
		return 0;
	}

	unsigned int Shader::GetHandle() const
	{
		return m_Handle;
	}

	const Shader::Type Shader::getType() const
	{
		return m_Type;
	}

	const std::string & Shader::getSource() const
	{
		return m_Source;
	}

	void Shader::SetSource(const std::string & source)
	{
		m_Source = source;
		m_IsCompiled = false;
	}

	Shader::Shader(Shader::Type type, const std::string & source)
		: m_Handle(0), m_Type(type), m_IsCompiled(false)
	{
		if(!source.empty())
			SetSource(source);
	}
}