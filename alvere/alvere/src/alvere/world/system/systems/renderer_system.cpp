#include "alvere/world/system/systems/renderer_system.hpp"

namespace alvere
{
	RendererSystem::RendererSystem( Renderer & renderer )
		: m_renderer( renderer )
	{
	}

	void RendererSystem::Render(C_Transform & transform, C_RenderableMesh & renderableMesh)
	{
		m_renderer.submit(renderableMesh.m_mesh, renderableMesh.m_material, transform->getWorldMatrix());
	}
}