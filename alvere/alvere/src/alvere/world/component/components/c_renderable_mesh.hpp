#pragma once

#include "alvere/graphics/material_instance.hpp"
#include "alvere/graphics/mesh.hpp"
#include "alvere/world/component/pooled_component.hpp"

namespace alvere
{
	class C_RenderableMesh : public PooledComponent<C_RenderableMesh>
	{
	public:

		MaterialInstance * m_material;

		Mesh * m_mesh;
	};
}