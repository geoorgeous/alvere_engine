#pragma once

#include "alvere/world/component/components/c_transform.hpp"
#include "alvere/world/component/components/c_renderable_mesh.hpp"
#include "alvere/graphics/renderer.hpp"
#include "alvere/world/system/query_rendered_system.hpp"

namespace alvere
{
	class S_Renderer : public virtual QueryRenderedSystem<const C_Transform, const C_RenderableMesh>
	{
	public:

		S_Renderer(Renderer & renderer);

		void Render(const C_Transform & transform, const C_RenderableMesh & mesh) override;

	private:

		Renderer & m_renderer;
	};
}