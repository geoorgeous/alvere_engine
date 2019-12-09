#pragma once

#include <memory>

#include "alvere/graphics/sprite_batcher.hpp"
#include "alvere/graphics/buffers.hpp"

#include "graphics_api/opengl/opengl_shader_program.hpp"
#include "graphics_api/opengl/opengl_vertex_array.hpp"

namespace alvere::graphics_api::opengl
{
	class SpriteBatcher : public alvere::SpriteBatcher
	{
	public:
		SpriteBatcher();
		~SpriteBatcher();

	protected:
		static BufferLayout s_vertexDataLayout;
		static unsigned int s_Indices[];
		static std::unique_ptr<Shader> s_vertexShader;
		static std::unique_ptr<Shader> s_fragmentShader;
		static std::unique_ptr<ShaderProgram> s_shaderProgram;

		float m_VertexData[ALV_MAX_SPRITEBATCH_SPRITES * (3 + 2 + 4 + 1) * sizeof(float)], *m_VPtr;
		const Texture * * m_Textures;
		int m_TexturesCount;
		alvere::VertexBuffer * m_VBO;
		alvere::IndexBuffer * m_EBO;
		VertexArray m_VAO;

		void processDrawCommandData(const DrawSpriteCommand& command) override;

		void Clear();
		void Draw();
		void flush() override;

	private:
		void InitStatic();
	};
}