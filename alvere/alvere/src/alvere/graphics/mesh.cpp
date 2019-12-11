#include "alvere/graphics/mesh.hpp"

#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "alvere/debug/logging.hpp"
#include "alvere/graphics/buffers.hpp"
#include "alvere/math/vector/vector_3.hpp"
#include "alvere/math/vector/vector_4.hpp"

namespace alvere
{
	Mesh * Mesh::Plane(float size)
	{
		return Box(size);
	}

	Mesh * Mesh::Plane(float width, float depth)
	{
		width *= 0.5f;
		float height = depth *= 0.0f;
		depth = 0;

		Vector3* positions = new Vector3[8]{
			Vector3{ -width, -height, -depth },
			Vector3{  width, -height, -depth },
			Vector3{ -width,  height, -depth },
			Vector3{  width,  height, -depth }
		};

		unsigned int* indices = new unsigned int[36]{
			0, 3, 2,
			3, 0, 1,
		};

		return new Mesh(positions, nullptr, nullptr, nullptr, nullptr, nullptr, 4, indices, 6);
	}

	Mesh * Mesh::Box(float size)
	{
		return Box(size, size, size);
	}

	Mesh * Mesh::Box(float width, float depth, float height)
	{
		width *= 0.5f;
		depth *= 0.5f;
		height *= 0.5f;

		Vector3 * positions = new Vector3[8]{
			Vector3{ -width, -height, -depth },
			Vector3{  width, -height, -depth },
			Vector3{ -width,  height, -depth },
			Vector3{  width,  height, -depth },
			Vector3{ -width, -height,  depth },
			Vector3{  width, -height,  depth },
			Vector3{ -width,  height,  depth },
			Vector3{  width,  height,  depth },
		};

		unsigned int * indices = new unsigned int[36]{
			0, 0, 0,
			0, 0, 0,

			0, 0, 0,
			0, 0, 0,

			0, 0, 0,
			0, 0, 0,

			0, 0, 0,
			0, 0, 0,

			0, 0, 0,
			0, 0, 0,

			0, 0, 0,
			0, 0, 0,
		};

		return new Mesh(positions, nullptr, nullptr, nullptr, nullptr, nullptr, 8, indices, 36);
	}

	Mesh * Mesh::Sphere(float size)
	{
		return Box(1.0f);
	}

	Mesh::TextureCoordinateData::~TextureCoordinateData()
	{
		for (int idx = 0; idx < count; idx++)
			delete[] data[idx];
		delete[] data;
		delete[] componentCount;
	}

	Mesh::ColourData::~ColourData()
	{
		for (int idx = 0; idx < count; idx++)
			delete[] data[idx];
		delete[] data;
	}

	Mesh::Mesh(const Vector3 * positions, const Vector3 * normals, const Vector3 * tangents, const Vector3 * bitangents, const TextureCoordinateData * texCoords, const ColourData * colours, unsigned int vertexCount, const unsigned int * indices, unsigned int indexCount)
		: m_VertexCount(vertexCount)
	{
		if (positions) { m_Positions = new Vector3[vertexCount]; std::memmove(m_Positions, positions, sizeof(Vector3) * vertexCount); }
		if (normals) std::memmove(m_Normals, normals, sizeof(Vector3) * vertexCount);
		if (tangents) std::memmove(m_Tangents, tangents, sizeof(Vector3) * vertexCount);
		if (bitangents) std::memmove(m_Bitangents, bitangents, sizeof(Vector3) * vertexCount);
		if (texCoords) std::memmove(m_TextureCoordinates, texCoords, sizeof(Vector3) * texCoords->count * vertexCount);
		if (colours) std::memmove(m_Colours, colours, sizeof(Vector4) * colours->count * vertexCount);
		if (indices) { m_ElementBuffer = IndexBuffer::New(indices, indexCount); }

		m_VertexBuffer = VertexBuffer::New(&m_Positions[0].x, m_VertexCount * 3);
		m_VertexBuffer->SetLayout({
			BufferElementProperties{Shader::DataType::Float3, "a_Position" }
			});
	}

	Mesh::Mesh(const char * filepath)
	{
		Assimp::Importer importer;

		const aiScene * scene = importer.ReadFile(filepath,
			//aiProcess_MakeLeftHanded |
			//aiProcess_FlipWindingOrder |
			aiProcess_GenUVCoords |
			//aiProcess_FlipUVs |
			//aiProcess_PreTransformVertices |
			//aiProcess_GenNormals |
			//aiProcess_CalcTangentSpace |
			//aiProcess_GenSmoothNormals |
			aiProcess_Triangulate |
			//aiProcess_FixInfacingNormals |
			//aiProcess_FindInvalidData |
			//aiProcess_ValidateDataStructure |
			0);

		if (!scene)
		{
			LogError("Failed to read mesh file \"%s\". More info: %s\n", filepath, importer.GetErrorString());
			return;
		}

		for (int idx = 0; idx < scene->mNumMeshes; idx++)
		{
			const aiMesh * mesh = scene->mMeshes[idx];

			m_VertexCount = mesh->mNumVertices;

			if (mesh->HasPositions())
			{
				m_Positions = new Vector3[mesh->mNumVertices];
				std::memmove(m_Positions, mesh->mVertices, mesh->mNumVertices * (sizeof(float) * 3));
			}
			if (mesh->HasNormals())
			{
				m_Normals = new Vector3[mesh->mNumVertices];
				std::memmove(m_Normals, mesh->mNormals, mesh->mNumVertices * (sizeof(float) * 3));
			}
			if (mesh->HasTangentsAndBitangents())
			{
				m_Tangents = new Vector3[mesh->mNumVertices];
				m_Bitangents = new Vector3[mesh->mNumVertices];
				std::memmove(m_Tangents, mesh->mTangents, mesh->mNumVertices * (sizeof(float) * 3));
				std::memmove(m_Bitangents, mesh->mBitangents, mesh->mNumVertices * (sizeof(float) * 3));
			}
			if (mesh->HasTextureCoords(0))
			{
				m_TextureCoordinates = new TextureCoordinateData;
				m_TextureCoordinates->count = mesh->GetNumUVChannels();
				m_TextureCoordinates->data = new Vector3*[m_TextureCoordinates->count];
				m_TextureCoordinates->componentCount = new unsigned int[m_TextureCoordinates->count];

				for (int idxx = 0; idxx < m_TextureCoordinates->count; idxx++)
				{
					m_TextureCoordinates->componentCount[idxx] = mesh->mNumUVComponents[idxx];
					m_TextureCoordinates->data[idxx] = new Vector3[mesh->mNumVertices];
					std::memmove(m_TextureCoordinates->data[idxx], mesh->mTextureCoords[idxx], mesh->mNumVertices * (sizeof(float) * 3));
				}
			}
			if (mesh->HasVertexColors(0))
			{
				m_Colours = new ColourData;
				m_Colours->count = mesh->GetNumColorChannels();
				m_Colours->data = new Vector4*[m_Colours->count];

				for (int idxx = 0; idxx < m_Colours->count; idxx++)
				{
					m_Colours->data[idxx] = new Vector4[mesh->mNumVertices];
					std::memmove(m_Colours->data[idxx], mesh->mColors[idxx], mesh->mNumVertices * (sizeof(float) * 4));
				}
			}
			if (mesh->HasFaces())
			{
				unsigned int * indices = new unsigned int[3 * mesh->mNumFaces];

				for (int idxx = 0, indicesIdx = 0; idxx < mesh->mNumFaces; idxx++)
				{

					for (int idxxx = 0; idxxx < mesh->mFaces[idxx].mNumIndices; idxxx++, indicesIdx++) indices[indicesIdx] = mesh->mFaces[idxx].mIndices[idxxx];
				}

				m_ElementBuffer = IndexBuffer::New(indices, mesh->mNumFaces * 3);

				delete[] indices;
			}
		}

		importer.FreeScene();

		float * data = new float[m_VertexCount * sizeof(float) * 6];

		for (int idx = 0; idx < m_VertexCount; idx++)
		{
			data[idx * 6 + 0] = m_Positions[idx].x;
			data[idx * 6 + 1] = m_Positions[idx].y;
			data[idx * 6 + 2] = m_Positions[idx].z;

			data[idx * 6 + 3] = m_TextureCoordinates->data[0][idx].x;
			data[idx * 6 + 4] = m_TextureCoordinates->data[0][idx].y;
			data[idx * 6 + 5] = m_TextureCoordinates->data[0][idx].z;
		}

		m_VertexBuffer = VertexBuffer::New(data, m_VertexCount * sizeof(float) * 6);

		m_VertexBuffer->SetLayout({
			BufferElementProperties{Shader::DataType::Float3, "a_position" },
			BufferElementProperties{Shader::DataType::Float3, "a_texCoords" },
		});
	}

	Mesh::~Mesh()
	{
		delete[] m_Positions;
		delete[] m_Normals;
		delete[] m_Tangents;
		delete[] m_Bitangents;
		delete m_TextureCoordinates;
		delete m_Colours;
	}

	VertexBuffer * Mesh::GetVertexBuffer() const
	{
		return m_VertexBuffer;
	}

	IndexBuffer * Mesh::GetElementBuffer() const
	{
		return m_ElementBuffer;
	}
}