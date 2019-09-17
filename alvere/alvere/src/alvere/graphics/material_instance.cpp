#include <glad/glad.h>

#include "alvere/graphics/material_instance.hpp"

namespace alvere
{
	void MaterialPropertyData<Shader::DataType::Float>::sendToShaderProgram(const ShaderProgram * shaderProgram) const
	{
		shaderProgram->sendUniformFloat1(m_propertyInfo->m_id, m_value);
	}

	void MaterialPropertyData<Shader::DataType::Float2>::sendToShaderProgram(const ShaderProgram * shaderProgram) const
	{
		shaderProgram->sendUniformFloat2(m_propertyInfo->m_id, m_value.x, m_value.y);
	}

	void MaterialPropertyData<Shader::DataType::Float3>::sendToShaderProgram(const ShaderProgram * shaderProgram) const
	{
		shaderProgram->sendUniformFloat3(m_propertyInfo->m_id, m_value.x, m_value.y, m_value.z);
	}

	void MaterialPropertyData<Shader::DataType::Float4>::sendToShaderProgram(const ShaderProgram * shaderProgram) const
	{
		shaderProgram->sendUniformFloat4(m_propertyInfo->m_id, m_value.x, m_value.y, m_value.z, m_value.w);
	}

	void MaterialPropertyData<Shader::DataType::Int>::sendToShaderProgram(const ShaderProgram * shaderProgram) const
	{
		shaderProgram->sendUniformInt1(m_propertyInfo->m_id, m_value);
	}

	void MaterialPropertyData<Shader::DataType::Int2>::sendToShaderProgram(const ShaderProgram * shaderProgram) const
	{
		shaderProgram->sendUniformInt2(m_propertyInfo->m_id, m_value[0], m_value[1]);
	}

	void MaterialPropertyData<Shader::DataType::Int3>::sendToShaderProgram(const ShaderProgram * shaderProgram) const
	{
		shaderProgram->sendUniformInt3(m_propertyInfo->m_id, m_value[0], m_value[1], m_value[2]);
	}

	void MaterialPropertyData<Shader::DataType::Int4>::sendToShaderProgram(const ShaderProgram * shaderProgram) const
	{
		shaderProgram->sendUniformInt4(m_propertyInfo->m_id, m_value[0], m_value[1], m_value[2], m_value[3]);
	}

	void MaterialPropertyData<Shader::DataType::Mat4x4>::sendToShaderProgram(const ShaderProgram * shaderProgram) const
	{
		shaderProgram->sendUniformMat4x4(m_propertyInfo->m_id, m_value);
	}

	void MaterialPropertyData<Shader::DataType::Sampler2D>::sendToShaderProgram(const ShaderProgram * shaderProgram) const
	{
		shaderProgram->sendUniformInt1(m_propertyInfo->m_id, m_openGLTextureUnitIndex);
		glActiveTexture(GL_TEXTURE0 + m_openGLTextureUnitIndex);
		if (m_value) m_value->bind();
	}

	MaterialInstance::MaterialInstance(const Material * baseMaterial)
		: m_baseMaterial(baseMaterial)
	{
		const std::vector<MaterialPropertyInfo> & propertiesInfo = baseMaterial->getPropertiesInfo();

		int openGLTextureUnitIndex = 0;

		for (const MaterialPropertyInfo & propertyInfo : propertiesInfo)
		{
			IMaterialPropertyData * newPropertyData;

			switch (propertyInfo.m_shaderDataType)
			{
				default: newPropertyData = new MaterialPropertyData<Shader::DataType::Float>; break;
				case Shader::DataType::Float2: newPropertyData = new MaterialPropertyData<Shader::DataType::Float2>; break;
				case Shader::DataType::Float3: newPropertyData = new MaterialPropertyData<Shader::DataType::Float3>; break;
				case Shader::DataType::Float4: newPropertyData = new MaterialPropertyData<Shader::DataType::Float4>; break;
				case Shader::DataType::Int: newPropertyData = new MaterialPropertyData<Shader::DataType::Int>; break;
				case Shader::DataType::Int2: newPropertyData = new MaterialPropertyData<Shader::DataType::Int2>; break;
				case Shader::DataType::Int3: newPropertyData = new MaterialPropertyData<Shader::DataType::Int3>; break;
				case Shader::DataType::Int4: newPropertyData = new MaterialPropertyData<Shader::DataType::Int4>; break;
				case Shader::DataType::Mat4x4: newPropertyData = new MaterialPropertyData<Shader::DataType::Mat4x4>; break;
				case Shader::DataType::Sampler2D:
					newPropertyData = new MaterialPropertyData<Shader::DataType::Sampler2D>;
					((MaterialPropertyData<Shader::DataType::Sampler2D> *)newPropertyData)->m_openGLTextureUnitIndex = openGLTextureUnitIndex++;
					break;
			}

			newPropertyData->m_propertyInfo = &propertyInfo;

			m_propertiesData.push_back(newPropertyData);
		}
	}

	const Material * MaterialInstance::getBaseMaterial() const
	{
		return m_baseMaterial;
	}

	void MaterialInstance::bind() const
	{
		m_baseMaterial->getShaderProgram()->bind();
	}

	void MaterialInstance::unbind() const
	{
		m_baseMaterial->getShaderProgram()->unbind();
	}

	void MaterialInstance::sendPropertiesToShader() const
	{
		const ShaderProgram * shaderProgram = m_baseMaterial->getShaderProgram();

		for (const IMaterialPropertyData * data : m_propertiesData)
			data->sendToShaderProgram(shaderProgram);
	}
}