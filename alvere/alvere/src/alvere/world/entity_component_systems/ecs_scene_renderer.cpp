#include "alvere/world/entity_component_systems/ecs_scene_renderer.hpp"

#include "alvere/utils/logging.hpp"
#include "alvere/world/entity.hpp"
#include "alvere/world/entity_components/ec_camera.hpp"
#include "alvere/world/entity_components/ec_rendered_mesh.hpp"
#include "alvere/world/entity_components/ec_transform.hpp"

#include <typeinfo>

namespace alvere
{
	ECSSceneRenderer::ECSSceneRenderer()
	{
		m_renderer = Renderer::New();
	}

	ECSSceneRenderer::~ECSSceneRenderer()
	{
		delete m_renderer;
	}

	void ECSSceneRenderer::updateSystems(std::unordered_map<std::type_index, std::pair<unsigned int, EntityComponent *>> & components, float timeStep)
	{
		auto cameras = components.find(typeid(ECCamera));

		if (cameras == components.end())
		{
			return;
		}

		const ECCamera & ec_camera = static_cast<ECCamera *>(cameras->second.second)[0];

		m_renderer->begin(ec_camera.camera);

		auto renderedMeshes = components.find(typeid(ECRenderedMesh));

		if (renderedMeshes != components.end())
		{
			std::pair<unsigned int, EntityComponent *> & pair = renderedMeshes->second;

			unsigned int count = components[typeid(ECRenderedMesh)].first;

			for (unsigned int i = 0; i < count; ++i)
			{
				const ECRenderedMesh & ec_renderedMesh = *static_cast<ECRenderedMesh *>(pair.second + i);
				const ECTransform & ec_transform = ec_renderedMesh.entity.transform();

				m_renderer->submit(ec_renderedMesh.m_mesh, ec_renderedMesh.m_material, ec_transform.transform.getWorldMatrix());
			}
		}

		m_renderer->end();
	}
}