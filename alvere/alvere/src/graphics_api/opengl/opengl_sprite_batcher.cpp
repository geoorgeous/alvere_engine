#include <glad/glad.h>

#include "alvere/assets.hpp"
#include "alvere/utils/file_reader.hpp"
#include "graphics_api/opengl/opengl_errors.hpp"
#include "graphics_api/opengl/opengl_sprite_batcher.hpp"
#include "graphics_api/opengl/opengl_vertex_array.hpp"

#define ALV_STRINGIFY(x) #x
#define ALV_STRINGIFY2(x) ALV_STRINGIFY(x)
#define ALV_OPENGL_MAX_TEXTUREUNITS_FRAGMENT 16

namespace alvere::graphics_api::opengl
{
	BufferLayout SpriteBatcher::s_vertexDataLayout = {
		BufferElementProperties{Shader::DataType::Float3, "a_Position" },
		BufferElementProperties{Shader::DataType::Float2, "a_TexCoords" },
		BufferElementProperties{Shader::DataType::Float4, "a_Colour" },
		BufferElementProperties{Shader::DataType::Int, "a_TextureIndex" }, };

	unsigned int SpriteBatcher::s_Indices[ALV_MAX_SPRITEBATCH_SPRITES * 6];

	Asset<Shader> SpriteBatcher::s_vertexShader;
	Asset<Shader> SpriteBatcher::s_fragmentShader;
	Asset<ShaderProgram> SpriteBatcher::s_shaderProgram;

	SpriteBatcher::SpriteBatcher()
		: m_VPtr(m_VertexData), m_TexturesCount(0)
	{
		if (s_shaderProgram == nullptr)
		{
			InitStatic();
		}

		int texCount = ALV_OPENGL_MAX_TEXTUREUNITS_FRAGMENT;
		m_Textures = new const Texture *[ALV_OPENGL_MAX_TEXTUREUNITS_FRAGMENT];

		m_VBO = alvere::VertexBuffer::New(m_VertexData, sizeof(m_VertexData));
		m_VBO->SetLayout(s_vertexDataLayout);

		m_EBO = alvere::IndexBuffer::New(s_Indices, ALV_MAX_SPRITEBATCH_SPRITES * 6);
		
		m_VAO.AddVertexBuffer(m_VBO);
		m_VAO.SetIndexBuffer(m_EBO);
	}

	SpriteBatcher::~SpriteBatcher()
	{
		delete[] m_Textures;
		delete m_VBO;
		delete m_EBO;
	}

	void SpriteBatcher::processDrawCommandData(const DrawSpriteCommand& command)
	{
		int textureIndex = -1;
		for (int idx = 0; idx < m_TexturesCount; idx++)
		{
			if (m_Textures[idx] == command.texture)
			{
				textureIndex = idx;
			}
		}

		if (textureIndex == -1)
		{
			if (m_TexturesCount == ALV_OPENGL_MAX_TEXTUREUNITS_FRAGMENT)
			{
				flush();
			}

			textureIndex = m_TexturesCount;
			m_Textures[textureIndex] = command.texture;
			m_TexturesCount++;
		}

		alvere::Vector2 topLeftTexCoords = command.texture->texCoords(command.source.getTopLeft());
		*m_VPtr = command.destination.getLeft();
		*++m_VPtr = command.destination.getTop();
		*++m_VPtr = command.sortLayer;
		*++m_VPtr = topLeftTexCoords.x;
		*++m_VPtr = topLeftTexCoords.y;
		*++m_VPtr = command.tint.x;
		*++m_VPtr = command.tint.y;
		*++m_VPtr = command.tint.z;
		*++m_VPtr = command.tint.w;
		*((int*)++m_VPtr) = textureIndex;

		alvere::Vector2 bottomLeftTexCoords = command.texture->texCoords(command.source.getBottomLeft());
		*++m_VPtr = command.destination.getLeft();
		*++m_VPtr = command.destination.getBottom();
		*++m_VPtr = command.sortLayer;
		*++m_VPtr = bottomLeftTexCoords.x;
		*++m_VPtr = bottomLeftTexCoords.y;
		*++m_VPtr = command.tint.x;
		*++m_VPtr = command.tint.y;
		*++m_VPtr = command.tint.z;
		*++m_VPtr = command.tint.w;
		*((int*)++m_VPtr) = textureIndex;

		alvere::Vector2 topRightTexCoords = command.texture->texCoords(command.source.getTopRight());
		*++m_VPtr = command.destination.getRight();
		*++m_VPtr = command.destination.getTop();
		*++m_VPtr = command.sortLayer;
		*++m_VPtr = topRightTexCoords.x;
		*++m_VPtr = topRightTexCoords.y;
		*++m_VPtr = command.tint.x;
		*++m_VPtr = command.tint.y;
		*++m_VPtr = command.tint.z;
		*++m_VPtr = command.tint.w;
		*((int*)++m_VPtr) = textureIndex;

		alvere::Vector2 bottomRightTexCoords = command.texture->texCoords(command.source.getBottomRight());
		*++m_VPtr = command.destination.getRight();
		*++m_VPtr = command.destination.getBottom();
		*++m_VPtr = command.sortLayer;
		*++m_VPtr = bottomRightTexCoords.x;
		*++m_VPtr = bottomRightTexCoords.y;
		*++m_VPtr = command.tint.x;
		*++m_VPtr = command.tint.y;
		*++m_VPtr = command.tint.z;
		*++m_VPtr = command.tint.w;
		*((int*)++m_VPtr) = textureIndex;

		if (++m_SpriteCount < ALV_MAX_SPRITEBATCH_SPRITES)
		{
			++m_VPtr;
		}
		else
		{
			flush();
		}
	}

	void SpriteBatcher::Draw()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		m_VBO->SetSubData(0, m_VertexData, m_SpriteCount * sizeof(float) * s_vertexDataLayout.GetStride());

		s_shaderProgram->bind();

		s_shaderProgram->sendUniformMat4x4("u_ProjectionView", *m_TransformationMatrix);

		for (int idx = 0; idx < m_TexturesCount; idx++)
		{
			glActiveTexture(GL_TEXTURE0 + idx);
			m_Textures[idx]->bind();
		}

		m_VAO.Bind();
		ALV_LOG_OPENGL_CALL(glDrawElements(GL_TRIANGLES, m_SpriteCount * 6, GL_UNSIGNED_INT, nullptr));
		m_VAO.Unbind();

		glDisable(GL_BLEND);
	}

	void SpriteBatcher::Clear()
	{
		m_VPtr = m_VertexData;
		m_SpriteCount = 0;
		m_TexturesCount = 0;
	}

	void SpriteBatcher::flush()
	{
		Draw();
		Clear();
	}

	void SpriteBatcher::InitStatic()
	{
		for (int idx = 0; idx < ALV_MAX_SPRITEBATCH_SPRITES; idx++)
		{
			int vertex = idx * 4;
			int index = idx * 6;
			s_Indices[index + 0] = vertex + 0;
			s_Indices[index + 1] = vertex + 1;
			s_Indices[index + 2] = vertex + 2;
			s_Indices[index + 3] = vertex + 2;
			s_Indices[index + 4] = vertex + 1;
			s_Indices[index + 5] = vertex + 3;
		}

		s_vertexShader = alvere::Shader::New(Shader::Type::Vertex, file::read("res/shaders/spritebatcher.vert"));

		s_fragmentShader = alvere::Shader::New(Shader::Type::Fragment, R"(
				#version 330 core

				uniform sampler2D u_Textures[)" ALV_STRINGIFY2(ALV_OPENGL_MAX_TEXTUREUNITS_FRAGMENT) R"(];

				in vec3 v_Position;
				in vec2 v_TexCoords;
				in vec4 v_Colour;
				flat in int v_TextureIndex;

				out vec4 f_Colour;

				void main()
				{
					f_Colour = texture(u_Textures[v_TextureIndex], v_TexCoords) * v_Colour;
					//f_Colour = vec4(v_Position, 1.0f);
					//f_Colour = vec4(v_TexCoords.x, 0.0f, v_TexCoords.y, 1.0f);
					//f_Colour = vec4(intBitsToFloat(v_TextureIndex) * 0.5f, 0.0f, 0.0f, 1.0f);
					//f_Colour = v_Colour;
				}
			)");

		s_shaderProgram = std::make_unique<ShaderProgram>();
		s_shaderProgram->SetShader(AssetRef<Shader>(s_vertexShader.get()));
		s_shaderProgram->SetShader(AssetRef<Shader>(s_fragmentShader.get()));
		s_shaderProgram->build();

		s_shaderProgram->bind();

		char numstr[16];
		for (int idx = 0; idx < ALV_OPENGL_MAX_TEXTUREUNITS_FRAGMENT; idx++)
		{
			sprintf(numstr, "u_Textures[%d]", idx);
			s_shaderProgram->sendUniformInt1(numstr, idx);
		}
	}
}

alvere::Asset<alvere::SpriteBatcher> alvere::SpriteBatcher::New()
{
	return alvere::Asset<alvere::graphics_api::opengl::SpriteBatcher>(new alvere::graphics_api::opengl::SpriteBatcher());
}