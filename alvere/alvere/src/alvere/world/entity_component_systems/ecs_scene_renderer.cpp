#include <typeinfo>

#include "alvere/world/entity_component_systems/ecs_scene_renderer.hpp"

#include "alvere/utils/logging.hpp"
#include "alvere/world/entity.hpp"
#include "alvere/world/entity_components/ec_camera.hpp"
#include "alvere/world/entity_components/ec_rendered_mesh.hpp"
#include "alvere/world/entity_components/ec_transform.hpp"

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

	void ECSSceneRenderer::updateSystems(EntityComponentMap & entityComponentMap, float timeStep)
	{
		ComponentCollection<ECCamera> * cameras = entityComponentMap.get<ECCamera>();

		if (cameras == nullptr)
		{
			return;
		}

		const ECCamera & ec_camera = cameras->components()[0];

		m_renderer->begin(ec_camera.camera);

		ComponentCollection<ECRenderedMesh> * renderedMeshesCollection = entityComponentMap.get<ECRenderedMesh>();

		if (renderedMeshesCollection != nullptr)
		{
			const std::vector<ECRenderedMesh> & renderedMeshes = renderedMeshesCollection->components();

			size_t count = renderedMeshes.size();

			for (size_t i = 0; i < count; ++i)
			{
				const ECRenderedMesh & ec_renderedMesh = renderedMeshes[i];
				const ECTransform & ec_transform = ec_renderedMesh.entity->transform();

				m_renderer->submit(ec_renderedMesh.mesh, ec_renderedMesh.material, ec_transform.transform.getWorldMatrix());
			}
		}

		m_renderer->end();
	}
}