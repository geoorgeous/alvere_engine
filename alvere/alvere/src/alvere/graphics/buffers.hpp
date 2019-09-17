#pragma once

#include <string>
#include <vector>

#include "alvere/graphics/shader.hpp"

namespace alvere
{
	struct BufferElementProperties
	{
		Shader::DataType m_DataType;
		std::string m_Name;
		unsigned int m_Size;
		unsigned int m_Offset;
		unsigned int m_ComponentCount;
		bool m_Normalise;

		BufferElementProperties();

		BufferElementProperties(Shader::DataType m_DataType, std::string name, bool normalise = false);
	};

	class BufferLayout
	{
	public:
		BufferLayout();
		BufferLayout(const std::vector<BufferElementProperties>& elements);
		BufferLayout(const std::initializer_list<BufferElementProperties>& elements);

		const std::vector<BufferElementProperties>& GetElements() const;
		unsigned int GetStride() const;

	private:
		std::vector<BufferElementProperties> m_Elements;
		unsigned int m_Stride;
	};

	class VertexBuffer
	{
	public:
		static VertexBuffer * New();
		static VertexBuffer * New(const float * vertexData, unsigned int size);

		VertexBuffer() = default;
		VertexBuffer(const VertexBuffer& vertexBuffer) = delete;
		VertexBuffer& operator=(const VertexBuffer& vertexBuffer) = delete;
		virtual ~VertexBuffer() = default;

		virtual void SetData(const float * vertexData, unsigned int size) = 0;
		virtual void SetSubData(unsigned int offset, const float * vertexData, unsigned int size) = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual const BufferLayout& GetLayout() const;
		virtual void SetLayout(const BufferLayout& layout);

	protected:
		BufferLayout m_Layout;
		unsigned int m_Size;
	};

	class IndexBuffer
	{
	public:
		static IndexBuffer * New(const unsigned int * indices, unsigned int count);

		IndexBuffer() = default;
		IndexBuffer(const IndexBuffer& vertexBuffer) = delete;
		IndexBuffer& operator=(const IndexBuffer& vertexBuffer) = delete;
		virtual ~IndexBuffer() = default;

		unsigned int GetCount() const;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

	protected:
		unsigned int m_Count;
	};

	class FrameBuffer
	{
	public:
		static FrameBuffer * New();

		FrameBuffer() = default;
		FrameBuffer(const FrameBuffer& frameBuffer) = delete;
		FrameBuffer& operator=(const FrameBuffer& frameBuffer) = delete;
		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
	};
}