#include <glad/glad.h>

#include "graphics_api/opengl/opengl_buffers.hpp"
#include "graphics_api/opengl/opengl_errors.hpp"

namespace alvere::graphics_api::opengl
{
	VertexBuffer::VertexBuffer()
	{
		m_Size = 0;
		glGenBuffers(1, &m_Handle);
	}

	VertexBuffer::VertexBuffer(const float * vertexData, unsigned int size)
		: VertexBuffer()
	{
		SetData(vertexData, size);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_Handle);
	}

	void VertexBuffer::SetData(const float * vertexData, unsigned int size)
	{
		m_Size = size;
		glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
		ALV_LOG_OPENGL_CALL(glBufferData(GL_ARRAY_BUFFER, size, vertexData, GL_DYNAMIC_DRAW));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetSubData(unsigned int offset, const float * vertexData, unsigned int size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
		ALV_LOG_OPENGL_CALL(glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertexData));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
	}

	void VertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	IndexBuffer::IndexBuffer(const unsigned int * indices, unsigned int count)
	{
		m_Count = count;
		glGenBuffers(1, &m_Handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
		ALV_LOG_OPENGL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_Handle);
	}

	void IndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
	}

	void IndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

alvere::VertexBuffer * alvere::VertexBuffer::New()
{
	return new alvere::graphics_api::opengl::VertexBuffer();
}

alvere::VertexBuffer * alvere::VertexBuffer::New(const float * vertexData, unsigned int size)
{
	return new alvere::graphics_api::opengl::VertexBuffer(vertexData, size);
}

alvere::IndexBuffer * alvere::IndexBuffer::New(const unsigned int * indices, unsigned int size)
{
	return new alvere::graphics_api::opengl::IndexBuffer(indices, size);
}