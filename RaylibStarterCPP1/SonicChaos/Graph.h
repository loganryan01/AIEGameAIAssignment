#pragma once

#include <vector>
#include <functional>
#include <algorithm>
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

		// What type of node it is
		bool doorNode = false;

		// Scores
		float gScore;

		// Parent node
		Node* parent;

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

	bool CheckList(std::list<Node*> List, Node* target)
	{
		// Check the closed list if the target exists
		for (auto it = List.begin(); it != List.end(); ++it)
		{
			if (target == *it)
			{
				return true;
			}
		}

		return false;
	}

	std::list<Node*> dijkstrasSearch(Node* startNode, Node* endNode)
	{
		std::list<Node*> path;

		if (startNode == nullptr)
		{
			//std::cout << "No Starting Node" << std::endl;
			return path;
		}
		else if (endNode == nullptr)
		{
			//std::cout << "No Ending Node" << std::endl;
			return path;
		}
		else if (startNode == endNode)
		{
			//std::cout << "The starting node and the ending node is the same" << std::endl;
			return path;
		}

		startNode->gScore = 0;
		startNode->parent = nullptr;

		std::list<Node*> openList;
		std::list<Node*> closedList;

		openList.push_back(startNode);

		while (!openList.empty())
		{
			openList.sort();

			Node* currentNode = openList.front();
			//std::cout << currentNode->gScore << std::endl;

			if (currentNode == endNode)
			{
				break;
			}

			openList.pop_front();
			closedList.push_back(currentNode);

			for (auto c : currentNode->connections)
			{
				if (!CheckList(closedList, c.to))
				{
					float gScore = currentNode->gScore + c.data;

					if (!CheckList(openList, c.to))
					{
						c.to->gScore = gScore;
						c.to->parent = currentNode;
						openList.push_back(c.to);
					}
					else if (gScore < c.to->gScore)
					{
						c.to->gScore = gScore;
						c.to->parent = currentNode;
					}
				}
			}
		}

		Node* currentNode = endNode;

		while (currentNode != nullptr)
		{
			path.push_front(currentNode);
			currentNode = currentNode->parent;
		}

		return path;
	}

protected:

	std::vector<Node*> m_nodes;
};