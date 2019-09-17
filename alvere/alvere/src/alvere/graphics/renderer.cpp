#include <algorithm>

#include "alvere/graphics/renderer.hpp"

namespace alvere
{
	void Renderer::begin(const Camera & camera)
	{
		m_projectionMatrix = &camera.GetProjectionMatrix();
		m_viewMatrix = &camera.GetViewMatrix();
	}

	void Renderer::end()
	{
		if (m_drawCommands.size() == 0)
			return;

		flush();
	}

	void Renderer::submit(const Mesh * mesh, const MaterialInstance * material, const Matrix4 & m_localTransform)
	{
		m_drawCommands.push_back(DrawCommand{ mesh, material, m_localTransform });
	}

	Renderer::DrawCommand & Renderer::DrawCommand::operator=(const DrawCommand & rhs)
	{
		m_mesh = rhs.m_mesh;
		m_material = rhs.m_material;
		m_localTransform = rhs.m_localTransform;
		return *this;
	}

	void Renderer::sortDrawCommands()
	{
		std::sort(m_drawCommands.begin(), m_drawCommands.end(), [](const DrawCommand & lhs, const DrawCommand & rhs)
			{
				return (intptr_t)lhs.m_mesh < (intptr_t)rhs.m_mesh;
			});
	}
}