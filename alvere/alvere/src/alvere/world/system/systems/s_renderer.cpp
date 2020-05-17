#include "alvere/world/system/systems/s_renderer.hpp"

namespace alvere
{
	S_Renderer::S_Renderer( Renderer & renderer )
		: m_renderer( renderer )
	{
	}

	void S_Renderer::Render(C_Transform & transform, C_RenderableMesh & renderableMesh)
	{
		m_renderer.submit(renderableMesh.m_mesh, renderableMesh.m_material, transform->getWorldMatrix());
	}
}