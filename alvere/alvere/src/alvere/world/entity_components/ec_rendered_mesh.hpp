#pragma once

#include "alvere/assets.hpp"
#include "alvere/graphics/material_instance.hpp"
#include "alvere/graphics/mesh.hpp"
#include "alvere/world/entity_component.hpp"

namespace alvere
{
	struct ECRenderedMesh : public EntityComponent
	{
		AssetRef<Mesh> mesh;
		
		AssetRef<MaterialInstance> material;
	};
}