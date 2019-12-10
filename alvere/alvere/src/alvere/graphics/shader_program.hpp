#pragma once

#include <memory>
#include <unordered_map>

#include "alvere/graphics/shader.hpp"
#include "alvere/math/matrix/matrix_4.hpp"

namespace alvere
{
	class ShaderProgram
	{
	public:

		static std::unique_ptr<ShaderProgram> New();

		ShaderProgram(const ShaderProgram& shaderProgram) = delete;

		const Shader * getShader(Shader::Type type) const;

		void SetShader(const Shader * shader);

		virtual bool build() = 0;

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		virtual void sendUniformInt1(const std::string & id, int value) const = 0;

		virtual void sendUniformInt2(const std::string & id, int value1, int value2) const = 0;

		virtual void sendUniformInt3(const std::string & id, int value1, int value2, int value3) const = 0;

		virtual void sendUniformInt4(const std::string & id, int value1, int value2, int value3, int value4) const = 0;

		virtual void sendUniformFloat1(const std::string & id, float value) const = 0;

		virtual void sendUniformFloat2(const std::string & id, float value1, float value2) const = 0;

		virtual void sendUniformFloat3(const std::string & id, float value1, float value2, float value3) const = 0;

		virtual void sendUniformFloat4(const std::string & id, float value1, float value2, float value3, float value4) const = 0;

		virtual void sendUniformMat4x4(const std::string & id, const Matrix4 & matrix) const = 0;

	protected:

		unsigned int m_handle;

		std::unordered_map<Shader::Type, const Shader *> m_shaders;

		ShaderProgram();
	};
}