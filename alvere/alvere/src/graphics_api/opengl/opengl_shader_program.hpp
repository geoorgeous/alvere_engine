#pragma once

#include "alvere/graphics/shader_program.hpp"
#include "alvere/math/matrices.hpp"

namespace alvere::graphics_api::opengl
{
	class ShaderProgram : public alvere::ShaderProgram
	{
	public:
		ShaderProgram();
		~ShaderProgram();

		bool build() override;
		void bind() const override;
		void unbind() const override;

		void sendUniformInt1(const std::string & id, int value) const override;

		void sendUniformInt2(const std::string & id, int value1, int value2) const override;

		void sendUniformInt3(const std::string & id, int value1, int value2, int value3) const override;

		void sendUniformInt4(const std::string & id, int value1, int value2, int value3, int value4) const override;

		void sendUniformFloat1(const std::string & id, float value) const override;

		void sendUniformFloat2(const std::string & id, float value1, float value2) const override;

		void sendUniformFloat3(const std::string & id, float value1, float value2, float value3) const override;

		void sendUniformFloat4(const std::string & id, float value1, float value2, float value3, float value4) const override;

		void sendUniformMat4x4(const std::string & id, const Matrix4 & matrix) const override;

	private:
		int GetUniformLocation(const std::string & id) const;
	};
}