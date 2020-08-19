/*--------------------------------------------------------------------
	File Name: Graph.h
	Purpose: Declare functions and macro definitions for source files.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------------------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------------------------------------------*/

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

	// struct for the nodes on the graph
	struct Node
	{
		// Data of the node
		TNodeData data = { 0 };

		// What type of node it is
		bool doorNode = false;
		bool chaosNode = false;
		bool masterNode = false;

		// Has the enemy already been here?
		bool beenHere = false;

		// Scores
		float gScore = 0.0f;
		float hScore = 0.0f;
		float fScore = 0.0f;

		// Parent node
		Node* parent = nullptr;

		// Connection to other node
		std::vector<Edge> connections;
	};

	// struct for the edges of the graph
	struct Edge
	{
		// The node that the edge is pointing to
		Node* to;

		// Data of the edge
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

	// Add a node to the graph
	Node* AddNode(const TNodeData& data)
	{
		Node* node = new Node();
		node->data = data;

		m_nodes.push_back(node);
		return node;
	}

	// Add an edge to the graph
	void AddEdge(Node* nodeA, Node* nodeB, const TEdgeData& data)
	{
		nodeA->connections.push_back({ nodeB, data });
	}

	// Add a barricade to the graph
	void AddBarricade(float xPos, float yPos)
	{
		m_barricades.push_back({ xPos, yPos});
	}

	// Get the graph's nodes
	const std::vector<Node*>& GetNodes()
	{
		return m_nodes;
	}
	// Get the graph's barricades
	const std::vector<Vector2>& GetBarricades()
	{
		return m_barricades;
	}

	// Check if a node alrady exists in the list
	bool CheckList(std::list<Node*> List, Node* target)
	{
		// Loop through the list
		for (auto it = List.begin(); it != List.end(); ++it)
		{
			// If the node is found in the list return true
			if (target == *it)
			{
				return true;
			}
		}

		// Otherwise, return false
		return false;
	}

	// Heuristic function for the A* search
	float Heuristic(Node* target, Node* endNode)
	{
		// Get the distance from the node that the Agent is on to the path's end node
		float distance = Vector2Distance(target->data, endNode->data);

		return distance;
	}

	// A* search algorithm
	std::list<Node*> AStarSearch(Node* startNode, Node* endNode)
	{
		// Create a path
		std::list<Node*> path;

		// Check if starting node does not equal to null
		if (startNode == nullptr)
		{
			return path;
		}
		// Check if ending node does not equal to null
		else if (endNode == nullptr)
		{
			return path;
		}
		// Check if starting node does not equal to ending node
		else if (startNode == endNode)
		{
			return path;
		}

		// Set the starting node g Score to 0 and parent to null
		startNode->gScore = 0;
		startNode->parent = nullptr;

		// Create an open list for all the nodes that need to be checked
		std::list<Node*> openList;

		// Create a closed list for all the nodes that have been checked
		std::list<Node*> closedList;

		// Add the starting node to the open list
		openList.push_back(startNode);

		// Loop through the open list while it is not empty
		while (!openList.empty())
		{
			// Sort the open list
			openList.sort();

			// Let the current node be the first item on the list
			Node* currentNode = openList.front();

			// If we are at the end break out of the loop
			if (currentNode == endNode)
			{
				break;
			}

			// Remove the first element from the open list and add it to closed list
			openList.pop_front();
			closedList.push_back(currentNode);

			// Loop through the node's edges
			for (auto c : currentNode->connections)
			{
				// If the edge's target node doesn't exist in the closed list
				if (!CheckList(closedList, c.to))
				{
					// Add the g Score of the current node and the edge's data
					float gScore = currentNode->gScore + c.data;

					// Heuristic the edge's target node with the end node
					float hScore = Heuristic(c.to, endNode);

					// Add the g score and h score
					float fScore = gScore + hScore;

					// If the edge's target node doesn't exist in the open list
					if (!CheckList(openList, c.to))
					{
						// Set its Scores to be the calculated Scores
						c.to->gScore = gScore;
						c.to->fScore = fScore;

						// Set its parent to be the current node
						c.to->parent = currentNode;

						// Add it to the open list
						openList.push_back(c.to);
					}
					// If the f Score less than the edge's target node's f Score
					else if (fScore < c.to->fScore)
					{
						// Do the same as the above code but don't add it to the open list
						c.to->gScore = gScore;
						c.to->fScore = fScore;
						c.to->parent = currentNode;
					}
				}
			}
		}

		// Create a new current node and set it to be the end node
		Node* currentNode = endNode;

		// Loop through until the current node is null
		while (currentNode != nullptr)
		{
			// Add the current node to the path list
			path.push_front(currentNode);

			// Set the current node to be its parent
			currentNode = currentNode->parent;
		}

		// Return the path
		return path;
	}

	// Dijstras Search algorithim
	std::list<Node*> dijkstrasSearch(Node* startNode, Node* endNode)
	{
		// Create a path
		std::list<Node*> path;

		// Check if starting node does not equal to null
		if (startNode == nullptr)
		{
			return path;
		}
		// Check if ending node does not equal to null
		else if (endNode == nullptr)
		{
			return path;
		}
		// Check if starting node does not equal to ending node
		else if (startNode == endNode)
		{
			return path;
		}

		// Set the starting node g Score to 0 and parent to null
		startNode->gScore = 0;
		startNode->parent = nullptr;

		// Create an open list for all the nodes that need to be checked
		std::list<Node*> openList;

		// Create a closed list for all the nodes that have been checked
		std::list<Node*> closedList;

		// Add the starting node to the open list
		openList.push_back(startNode);

		// Loop through the open list while it is not empty
		while (!openList.empty())
		{
			// Sort the open list
			openList.sort();

			// Let the current node be the first item on the list
			Node* currentNode = openList.front();

			// If we are at the end break out of the loop
			if (currentNode == endNode)
			{
				break;
			}

			// Remove the first element from the open list and add it to closed list
			openList.pop_front();
			closedList.push_back(currentNode);

			// Loop through the node's edges
			for (auto c : currentNode->connections)
			{
				// If the edge's target node doesn't exist in the closed list
				if (!CheckList(closedList, c.to))
				{
					// Add the g Score of the current node and the edge's data
					float gScore = currentNode->gScore + c.data;

					// If the edge's target node doesn't exist in the open list
					if (!CheckList(openList, c.to))
					{
						// Set its Scores to be the calculated Scores
						c.to->gScore = gScore;

						// Set its parent to be the current node
						c.to->parent = currentNode;

						// Add it to the open list
						openList.push_back(c.to);
					}
					// If the g Score less than the edge's target node's g Score
					else if (gScore < c.to->gScore)
					{
						// Do the same as the above code but don't add it to the open list
						c.to->gScore = gScore;
						c.to->parent = currentNode;
					}
				}
			}
		}

		// Create a new current node and set it to be the end node
		Node* currentNode = endNode;

		// Loop through until the current node is null
		while (currentNode != nullptr)
		{
			// Add the current node to the path list
			path.push_front(currentNode);

			// Set the current node to be its parent
			currentNode = currentNode->parent;
		}

		// Return the path
		return path;
	}

	// Depth First Search algorithim
	Node* DepthFirstSearch(Node* start,
		std::function<bool(Node*)> predicate)
	{
		// Create an open list for all the nodes that need to be checked
		std::list<Node*> openList;

		// Create a closed list for all the nodes that have been checked
		std::list<Node*> closedList;

		// Add the starting node to the open list
		openList.push_front(start);

		// Loop through the open list while it is not empty
		while (!openList.empty())
		{
			// Let the current node be the first node of the open list
			Node* currentNode = openList.front();

			// If the current node meets predicate
			if (predicate(currentNode))
			{
				// Then return that node
				return currentNode;
			}

			// Remove the first element from the open list and add it to closed list
			openList.pop_front();
			closedList.push_back(currentNode);

			// Loop through the node's edges
			for (auto c : currentNode->connections)
			{
				// If the node exists in the lists
				if (CheckList(openList, c.to) || CheckList(closedList, c.to))
				{
					// Go to the next node
					continue;
				}
				else 
				{
					// Otherwise add it to the open list
					openList.push_front(c.to);
				}
			}
		}
	}

protected:
	// The nodes of the graph
	std::vector<Node*> m_nodes;

	// The barricades of the graph
	std::vector<Vector2> m_barricades;
};