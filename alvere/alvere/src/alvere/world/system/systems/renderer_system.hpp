#pragma once

#include "alvere/world/component/components/c_transform.hpp"
#include "alvere/world/component/components/c_renderable_mesh.hpp"
#include "alvere/graphics/renderer.hpp"
#include "alvere/world/system/query_rendered_system.hpp"

namespace alvere
{
	class RendererSystem : public virtual QueryRenderedSystem<C_Transform, C_RenderableMesh>
	{
	public:

		RendererSystem(Renderer & renderer);

		void Render(C_Transform & transform, C_RenderableMesh & mesh) override;

	private:

		Renderer & m_renderer;
	};
}