#pragma once

#include <vector>

#include "alvere/assets.hpp"
#include "alvere/graphics/material_instance.hpp"
#include "alvere/graphics/mesh.hpp"
#include "alvere/graphics/renderer_light.hpp"
#include "alvere/math/matrix/matrix_4.hpp"
#include "alvere/math/matrix/transformations.hpp"
#include "alvere/world/application/transform.hpp"

namespace alvere
{
	class RendererSceneGraph
	{
	public:

		class Node
		{
		public:

			inline Transform & localTransform()
			{
				return m_localTransform;
			}

			inline virtual bool operator==(const Node & rhs) const
			{
				return m_localTransform == rhs.m_localTransform && typeid(*this) == typeid(rhs) && isEqual(rhs);
			}

		private:

			Transform m_localTransform;

			virtual bool isEqual(const Node & other) const = 0;
		};

		class ObjectNode : public Node
		{
		public:

			inline const AssetRef<Mesh> getMesh() const
			{
				return mesh;
			}

			inline const AssetRef<MaterialInstance> getMaterial() const
			{
				return material;
			}

			inline void setMesh(AssetRef<Mesh> mesh)
			{
				mesh = mesh;
			}

			inline void setMaterial(AssetRef<MaterialInstance> material)
			{
				material = material;
			}

		private:

			AssetRef<Mesh> mesh;

			AssetRef<MaterialInstance> material;

			inline bool isEqual(const Node & other) const override
			{
				const ObjectNode & otherObjectNode = static_cast<const ObjectNode &>(other);
				return
					mesh == otherObjectNode.mesh &&
					material == otherObjectNode.material;
			}
		};

		class LightNode : public Node
		{
		public:

			inline RendererLight * lightInfo()
			{
				return m_light;
			}

		private:

			RendererLight * m_light;

			bool isEqual(const Node & other) const override
			{
				const LightNode & otherLightNode = reinterpret_cast<const LightNode &>(other);
				return *m_light == *otherLightNode.m_light;
			}
		};

		~RendererSceneGraph();

		inline const std::vector<Node *> & getNodes() const
		{
			return m_nodes;
		}

		Node * newNode();

		void deleteNode(Node * node);

	private:

		std::vector<Node *> m_nodes;

		std::vector<RendererLight> m_lights;
	};
}