/*-----------------------------
	File Name: Graph2D.cpp
	Purpose: Create a 2D Graph.
	Author: Logan Ryan
	Modified: 19 August 2020
-------------------------------
	Copyright 2020 Logan Ryan.
-----------------------------*/
#include "Graph2D.h"

//------------
// Constructor
//------------
Graph2D::Graph2D()
{

}

//-----------
// Destructor
//-----------
Graph2D::~Graph2D()
{

}

//------------------
// Draw the 2D graph
//------------------
void Graph2D::Draw()
{
	// Draw all nodes
	for (auto node : GetNodes())
	{
		if (node->doorNode == true)
		{
			DrawCircle((int)node->data.x, (int)node->data.y, 8, GREEN);
			DrawCircleLines((int)node->data.x, (int)node->data.y, 8, DARKGREEN);
		}
		else if (node->chaosNode == true)
		{
			DrawCircle((int)node->data.x, (int)node->data.y, 8, BLUE);
			DrawCircleLines((int)node->data.x, (int)node->data.y, 8, DARKBLUE);
		}
		else if (node->masterNode == true)
		{
			DrawCircle((int)node->data.x, (int)node->data.y, 8, BROWN);
			DrawCircleLines((int)node->data.x, (int)node->data.y, 8, DARKBROWN);
		}
		else
		{
			DrawCircle((int)node->data.x, (int)node->data.y, 8, GRAY);
			DrawCircleLines((int)node->data.x, (int)node->data.y, 8, LIGHTGRAY);
		}
	}
}

//--------------
// Draw the path
//--------------
void Graph2D::DrawPath(std::list<Node*>& path)
{
	for (auto node : path)
	{
		for (auto connection : node->connections)
		{
			DrawLine((int)node->data.x, (int)node->data.y, (int)connection.to->data.x, (int)connection.to->data.y, RED);
		}
	}
}

//---------------------------------------------------
// Set node to be a door node
//	xPos (float): What is the x position of the node?
//	yPos (float): What is the y position of the node?
//---------------------------------------------------
void Graph2D::SetDoorNode(float xPos, float yPos)
{
	Node* node = new Node();
	node->data = { xPos, yPos };
	node->doorNode = true;

	m_nodes.push_back(node);
}

//---------------------------------------------------
// Set node to be a Chaos Emerald node
//	xPos (float): What is the x position of the node?
//	yPos (float): What is the y position of the node?
//---------------------------------------------------
void Graph2D::SetChaosNode(float xPos, float yPos)
{
	Node* node = new Node();
	node->data = { xPos, yPos };
	node->chaosNode = true;

	m_nodes.push_back(node);
}

//---------------------------------------------------
// Set node to be a Master Emerald node
//	xPos (float): What is the x position of the node?
//	yPos (float): What is the y position of the node?
//---------------------------------------------------
void Graph2D::SetMasterNode(float xPos, float yPos)
{
	Node* node = new Node();
	node->data = { xPos, yPos };
	node->masterNode = true;

	m_nodes.push_back(node);
}

//---------------------------------------------------------
// Set the edges of the node
//	start (Graph2D::Node*): What is the starting node?
//	radius (float): What is the radius of the nearby nodes?
//---------------------------------------------------------
void Graph2D::SetEdges(Graph2D::Node* start, float radius)
{
	float numberOfNodes = 1.0f;
	for (auto node : m_nodes)
	{
		numberOfNodes += 1.0f;
		if (node != start)
		{
			float dist = Vector2Distance(start->data, node->data);
			if (dist < radius)
			{
				AddEdge(start, node, numberOfNodes);
			}
		}
	}
}