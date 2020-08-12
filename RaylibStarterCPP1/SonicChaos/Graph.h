#pragma once

#include <vector>
#include <functional>
#include <algorithm>
#include <iterator>
#include <list>

template<class TNodeData, class TEdgeData>
class Graph
{
public:

	struct Node;
	struct Edge;

	struct Node
	{
		TNodeData data;
		bool doorNode = false;

		std::vector<Edge> connections;
	};

	struct Edge
	{
		Node* to;
		TEdgeData data;
	};

public:

	// Constructor
	Graph()
	{

	}

	// Destructor
	virtual ~Graph()
	{
		for (auto node : m_nodes)
		{
			delete node;
		}

		m_nodes.clear();
	}

	// Add Node
	Node* AddNode(const TNodeData& data)
	{
		Node* node = new Node();
		node->data = data;

		m_nodes.push_back(node);
		return node;
	}

	// Add edge
	void AddEdge(Node* nodeA, Node* nodeB, const TEdgeData& data)
	{
		nodeA->connections.push_back({ nodeB, data });
	}

	// Get nodes
	const std::vector<Node*>& GetNodes()
	{
		return m_nodes;
	}

protected:

	std::vector<Node*> m_nodes;
};