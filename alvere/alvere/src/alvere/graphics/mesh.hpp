#pragma once

#include "alvere/graphics/buffers.hpp"

namespace alvere
{
	struct Vector3;

	struct Vector4;

	class Mesh
	{
	public:

		struct TextureCoordinateData
		{
			Vector3 ** data;
			unsigned int * componentCount;
			unsigned int count;

			~TextureCoordinateData();
		};

		struct ColourData
		{
			Vector4 ** data;
			unsigned int count;

			~ColourData();
		};

		static Mesh * Plane(float size = 1.0f);

		static Mesh * Plane(float width, float depth);

		static Mesh * Box(float size = 1.0f);

		static Mesh * Box(float width, float depth, float height);

		static Mesh * Sphere(float size = 1.0f);

		Mesh(const Vector3 * positions, const Vector3 * normals, const Vector3 * tangents, const Vector3 * bitangents, const TextureCoordinateData * texCoords, const ColourData * colours, unsigned int vertexCount, const unsigned int * indices, unsigned int indexCount);

        Mesh(const char * filepath);

        ~Mesh();

        VertexBuffer * GetVertexBuffer() const;

        IndexBuffer * GetElementBuffer() const;

	private:
		Vector3 * m_Positions;

		Vector3 * m_Normals;

		Vector3 * m_Tangents;

		Vector3 * m_Bitangents;

		TextureCoordinateData * m_TextureCoordinates;

		ColourData * m_Colours;

		unsigned int m_VertexCount;

		VertexBuffer * m_VertexBuffer;

		IndexBuffer * m_ElementBuffer;
	};
}
