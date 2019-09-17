#include <glad/glad.h>

#include "alvere/graphics/shader.hpp"

#include "graphics_api/opengl/opengl_errors.hpp"
#include "graphics_api/opengl/opengl_vertex_array.hpp"

namespace alvere::graphics_api::opengl
{
	static GLenum ShaderDataTypeToOpenGLTypeEnum(Shader::DataType dataType)
	{
		switch (dataType)
		{
		case Shader::DataType::Float:
		case Shader::DataType::Float2:
		case Shader::DataType::Float3:
		case Shader::DataType::Float4: return GL_FLOAT;
		case Shader::DataType::Int:
		case Shader::DataType::Int2:
		case Shader::DataType::Int3:
		case Shader::DataType::Int4: return GL_INT;
		}
		return 0;
	}

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_Handle);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_Handle);
	}

	void VertexArray::Bind()
	{
		glBindVertexArray(m_Handle);
	}

	void VertexArray::Unbind()
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(VertexBuffer * buffer)
	{
		glBindVertexArray(m_Handle);

		buffer->Bind();

		unsigned int index = 0;
		const BufferLayout& layout = buffer->GetLayout();
		const std::vector<BufferElementProperties>& layoutElements = layout.GetElements();
		for (const BufferElementProperties& element : layoutElements)
		{
			glEnableVertexAttribArray(index);
			if (element.m_DataType < Shader::DataType::Int || element.m_DataType > Shader::DataType::Int4)
			{
				ALV_LOG_OPENGL_CALL(glVertexAttribPointer(
					index,
					element.m_ComponentCount,
					ShaderDataTypeToOpenGLTypeEnum(element.m_DataType),
					element.m_Normalise,
					layout.GetStride(),
					(const void*)element.m_Offset));
			}
			else
			{
				ALV_LOG_OPENGL_CALL(glVertexAttribIPointer(
					index,
					element.m_ComponentCount,
					ShaderDataTypeToOpenGLTypeEnum(element.m_DataType),
					layout.GetStride(),
					(const void*)element.m_Offset));
			}
			index++;
		}

		m_VertexBuffers.push_back(buffer);

		glBindVertexArray(0);
	}

	const std::vector<VertexBuffer *>& VertexArray::GetVertexBuffers() const
	{
		return m_VertexBuffers;
	}

	void VertexArray::SetIndexBuffer(IndexBuffer * buffer)
	{
		glBindVertexArray(m_Handle);

		buffer->Bind();

		m_IndexBuffer = buffer;

		glBindVertexArray(0);
	}

	const IndexBuffer * VertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}
}