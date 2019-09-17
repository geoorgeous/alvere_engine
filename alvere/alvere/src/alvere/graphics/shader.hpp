#pragma once

#include <string>

#include "alvere/assets.hpp"

namespace alvere
{
	class Shader
	{
	public:
		enum class Type
		{
			Vertex,
			Geometry,
			Fragment
		};

		enum class DataType
		{
			Float,
			Float2,
			Float3,
			Float4,
			Int,
			Int2,
			Int3,
			Int4,
			Mat4x4,
			Sampler2D
		};

		static Asset<Shader> New(Shader::Type type, const std::string& source);

		static unsigned int GetDataTypeSize(Shader::DataType dataType);

		static unsigned int GetDataTypeComponentCount(Shader::DataType dataType);

		Shader(const Shader& shader) = delete;

		unsigned int GetHandle() const;

		const Shader::Type getType() const;

		const std::string& getSource() const;

		void SetSource(const std::string& source);

		virtual bool Compile() = 0;

	protected:

		unsigned int m_Handle;

		Shader::Type m_Type;

		std::string m_Source;

		bool m_IsCompiled;

		Shader(Shader::Type type, const std::string& source);
	};
}