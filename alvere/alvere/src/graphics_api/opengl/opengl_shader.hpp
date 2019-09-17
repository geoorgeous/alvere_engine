#pragma once

#include "alvere/graphics/shader.hpp"

namespace alvere::graphics_api::opengl
{
	class Shader : public alvere::Shader
	{
	public:
		Shader(Shader::Type type, const std::string& source);

		~Shader();

		bool Compile() override;
	};
}