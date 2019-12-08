#pragma once

#include "alvere/graphics/buffers.hpp"

namespace alvere::graphics_api::opengl
{
	class VertexBuffer : public alvere::VertexBuffer
	{
	public:
		VertexBuffer();
		VertexBuffer(const float * vertexData, unsigned int size);
		~VertexBuffer();

		void SetData(const float * vertexData, unsigned int size) override;
		void SetSubData(unsigned int offset, const float * vertexData, unsigned int size) override;
		void Bind() override;
		void Unbind() override;

	private:
		unsigned int m_Handle;
	};

	class IndexBuffer : public alvere::IndexBuffer
	{
	public:
		IndexBuffer(const unsigned int * indices, unsigned int count);
		~IndexBuffer();

		void Bind() override;
		void Unbind() override;

	private:
		unsigned int m_Handle;
	};
}