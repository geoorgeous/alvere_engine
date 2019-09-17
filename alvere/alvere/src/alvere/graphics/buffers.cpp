#include "alvere/graphics/buffers.hpp"

namespace alvere
{
	BufferElementProperties::BufferElementProperties() { }

	BufferElementProperties::BufferElementProperties(Shader::DataType dataType, std::string name, bool normalise)
		: m_DataType(dataType), m_Name(name), m_Size(Shader::GetDataTypeSize(dataType)), m_Offset(0), m_ComponentCount(Shader::GetDataTypeComponentCount(dataType)), m_Normalise(normalise) { }

	BufferLayout::BufferLayout() { }

	BufferLayout::BufferLayout(const std::vector<BufferElementProperties>& elements)
		: m_Elements(elements)
	{
		unsigned int offset = 0;
		m_Stride = 0;
		for (BufferElementProperties& element : m_Elements)
		{
			element.m_Offset = offset;
			offset += element.m_Size;
			m_Stride += element.m_Size;
		}
	}

	BufferLayout::BufferLayout(const std::initializer_list<BufferElementProperties>& elements)
		: BufferLayout((const std::vector<BufferElementProperties>&)elements) { }
	
	const std::vector<BufferElementProperties>& BufferLayout::GetElements() const
	{
		return m_Elements;
	}

	unsigned int BufferLayout::GetStride() const
	{
		return m_Stride;
	}

	const BufferLayout& VertexBuffer::GetLayout() const
	{
		return m_Layout;
	}

	void VertexBuffer::SetLayout(const BufferLayout& layout)
	{
		m_Layout = layout;
	}

	unsigned int IndexBuffer::GetCount() const
	{
		return m_Count;
	}
}