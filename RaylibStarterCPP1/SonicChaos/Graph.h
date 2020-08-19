#pragma once

#include <vector>
#include <functional>
#include <algorithm>
#include <list>
#include <stack>

#include "raylib.h"

template<class TNodeData, class TEdgeData>
class Graph
{
public:

	struct Node;
	struct Edge;

	struct Node
	{
		TNodeData data = { 0 };

		// What type of node it is
		bool doorNode = false;
		bool chaosNode = false;
		bool masterNode = false;

		// Have I already been here?
		bool beenHere = false;

		// Was there a Chaos Emerald in the room?
		// bool chaosRoom = false;

		// Scores
		float gScore = 0.0f;
		float hScore = 0.0f;
		float fScore = 0.0f;

		// Parent node
		Node* parent = nullptr;

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

	void AddBarricade(float xPos, float yPos)
	{
		m_barricades.push_back({ xPos, yPos});
	}

	// Get nodes
	const std::vector<Node*>& GetNodes()
	{
		return m_nodes;
	}

	const std::vector<Vector2>& GetBarricades()
	{
		return m_barricades;
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

	float Heuristic(Node* target, Node* endNode)
	{
		float distance = Vector2Distance(target->data, endNode->data);

		return distance;
	}

	std::list<Node*> AStarSearch(Node* startNode, Node* endNode)
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
			//std::cout << currentNode->fScore << std::endl;

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
					float hScore = Heuristic(c.to, endNode);
					float fScore = gScore + hScore;

					if (!CheckList(openList, c.to))
					{
						c.to->gScore = gScore;
						c.to->fScore = fScore;
						c.to->parent = currentNode;
						openList.push_back(c.to);
					}
					else if (fScore < c.to->fScore)
					{
						c.to->gScore = gScore;
						c.to->fScore = fScore;
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

	bool CheckClosedList(std::list<Node*> closedList, Node* target)
	{
		// Check the closed list if the target exists
		for (auto it = closedList.begin(); it != closedList.end(); ++it)
		{
			if (target == *it)
			{
				return true;
			}
		}

		return false;
	}

	Node* DepthFirstSearch(Node* start,
		std::function<bool(Node*)> predicate)
	{
		std::list<Node*> openList;
		std::list<Node*> closedList;

		openList.push_front(start);

		while (!openList.empty())
		{
			Node* currentVertex = openList.front();

			if (predicate(currentVertex))
			{
				return currentVertex;
			}

			openList.pop_front();
			closedList.push_back(currentVertex);

			for (auto c : currentVertex->connections)
			{
				if (CheckList(openList, c.to) || CheckList(closedList, c.to))
				{
					continue;
				}
				else 
				{
					openList.push_front(c.to);
				}
			}
		}
	}

protected:

	std::vector<Node*> m_nodes;
	std::vector<Vector2> m_barricades;
};