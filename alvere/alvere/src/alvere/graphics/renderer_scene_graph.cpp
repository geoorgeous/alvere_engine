#include "alvere/graphics/renderer_scene_graph.hpp"

#include <algorithm>

namespace alvere
{
	RendererSceneGraph::~RendererSceneGraph()
	{
		for (Node * n : m_nodes)
			delete n;
	}

	RendererSceneGraph::Node * RendererSceneGraph::newNode()
	{
		//m_nodes.push_back(new Node);

		return m_nodes.back();
	}

	void RendererSceneGraph::deleteNode(Node * node)
	{
		std::vector<Node *>::iterator it = std::find(m_nodes.begin(), m_nodes.end(), node);
		
		if (it != m_nodes.end())
			m_nodes.erase(it);

		delete node;
	}
}