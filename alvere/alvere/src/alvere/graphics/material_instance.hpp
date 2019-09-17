#pragma once

#include "alvere/graphics/material.hpp"
#include "alvere/graphics/texture.hpp"
#include "alvere/math/matrix/matrix_4.hpp"
#include "alvere/math/vectors.hpp"

namespace alvere
{
	class MaterialInstance;

	struct IMaterialPropertyData
	{
		const MaterialPropertyInfo * m_propertyInfo;

	protected:

		friend alvere::MaterialInstance;

		virtual void sendToShaderProgram(const ShaderProgram * shaderProgram) const = 0;
	};

	template <Shader::DataType ShaderDataType>
	struct MaterialPropertyData : IMaterialPropertyData { };

	template<>
	struct MaterialPropertyData<Shader::DataType::Float> : IMaterialPropertyData
	{
		float m_value = 0.0f;

	private:

		void sendToShaderProgram(const ShaderProgram * shaderProgram) const override;
	};

	template<>
	struct MaterialPropertyData<Shader::DataType::Float2> : IMaterialPropertyData
	{
		Vector2 m_value;

	private:

		void sendToShaderProgram(const ShaderProgram * shaderProgram) const override;
	};

	template<>
	struct MaterialPropertyData<Shader::DataType::Float3> : IMaterialPropertyData
	{
		Vector3 m_value;

	private:

		void sendToShaderProgram(const ShaderProgram * shaderProgram) const override;
	};

	template<>
	struct MaterialPropertyData<Shader::DataType::Float4> : IMaterialPropertyData
	{
		Vector4 m_value;

	private:

		void sendToShaderProgram(const ShaderProgram * shaderProgram) const override;
	};

	template<>
	struct MaterialPropertyData<Shader::DataType::Int> : IMaterialPropertyData
	{
		int m_value = 0;

	private:

		void sendToShaderProgram(const ShaderProgram * shaderProgram) const override;
	};

	template<>
	struct MaterialPropertyData<Shader::DataType::Int2> : IMaterialPropertyData
	{
		Vector<int, 2> m_value;

	private:

		void sendToShaderProgram(const ShaderProgram * shaderProgram) const override;
	};

	template<>
	struct MaterialPropertyData<Shader::DataType::Int3> : IMaterialPropertyData
	{
		Vector<int, 3> m_value;

	private:

		void sendToShaderProgram(const ShaderProgram * shaderProgram) const override;
	};

	template<>
	struct MaterialPropertyData<Shader::DataType::Int4> : IMaterialPropertyData
	{
		Vector<int, 4> m_value;

	private:

		void sendToShaderProgram(const ShaderProgram * shaderProgram) const override;
	};

	template<>
	struct MaterialPropertyData<Shader::DataType::Mat4x4> : IMaterialPropertyData
	{
		Matrix4 m_value;

	private:

		void sendToShaderProgram(const ShaderProgram * shaderProgram) const override;
	};

	template<>
	struct MaterialPropertyData<Shader::DataType::Sampler2D> : IMaterialPropertyData
	{
		friend class MaterialInstance;

		Texture * m_value = nullptr;

	private:

		int m_openGLTextureUnitIndex = 0;

		void sendToShaderProgram(const ShaderProgram * shaderProgram) const override;
	}; 



	class MaterialInstance
	{
	public:

		MaterialInstance(const Material * baseMaterial);

		const Material * getBaseMaterial() const;

		template<Shader::DataType ShaderDataType>
		MaterialPropertyData<ShaderDataType> * get(const std::string & id)
		{
			const std::vector<MaterialPropertyInfo> & propertiesInfo = m_baseMaterial->getPropertiesInfo();

			for (IMaterialPropertyData * propertyData : m_propertiesData)
				if (propertyData->m_propertyInfo->m_id == id)
				{
					if (propertyData->m_propertyInfo->m_shaderDataType == ShaderDataType)
						return (MaterialPropertyData<ShaderDataType> *)propertyData;

					break;
				}

			return nullptr;
		}

		void bind() const;

		void unbind() const;

		void sendPropertiesToShader() const;

	private:

		const Material * m_baseMaterial;

		std::vector<IMaterialPropertyData *> m_propertiesData;
	};
}