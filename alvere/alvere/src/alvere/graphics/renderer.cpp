#include "alvere/graphics/renderer.hpp"

#include <algorithm>

namespace alvere
{
	void Renderer::begin(const Camera & camera)
	{
		m_projectionMatrix = &camera.getProjectionMatrix();
		m_viewMatrix = &camera.getViewMatrix();
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
		mesh = rhs.mesh;
		material = rhs.material;
		m_localTransform = rhs.m_localTransform;
		return *this;
	}

	void Renderer::sortDrawCommands()
	{
		std::sort(m_drawCommands.begin(), m_drawCommands.end(), [](const DrawCommand & lhs, const DrawCommand & rhs)
		{
			return (intptr_t)lhs.mesh < (intptr_t)rhs.mesh;
		});
	}
}