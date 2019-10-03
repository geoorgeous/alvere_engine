#include "alvere/world/system/systems/renderer_system.hpp"

namespace alvere
{
	void RendererSystem::setRenderer(Renderer * renderer)
	{
		m_renderer = renderer;
	}

	void RendererSystem::Render(C_Transform & transform, C_RenderableMesh & renderableMesh)
	{
		m_renderer->submit(renderableMesh.m_mesh, renderableMesh.m_material, transform->getWorldMatrix());
	}
}