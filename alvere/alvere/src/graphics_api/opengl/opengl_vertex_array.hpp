#pragma once

#include <vector>

#include "alvere/graphics/buffers.hpp"

namespace alvere::graphics_api::opengl
{
	class VertexArray
	{
	public:
		VertexArray();
		VertexArray(const VertexArray& vertexBuffer) = delete;
		VertexArray& operator=(const VertexArray& vertexBuffer) = delete;
		~VertexArray();

		void Bind();
		void Unbind();

		void AddVertexBuffer(VertexBuffer * buffer);
		void SetIndexBuffer(IndexBuffer * buffer);

		const std::vector<VertexBuffer *>& GetVertexBuffers() const;
		const IndexBuffer * GetIndexBuffer() const;

	private:
		unsigned int m_Handle;
		std::vector<VertexBuffer *> m_VertexBuffers;
		IndexBuffer * m_IndexBuffer;
	};
}