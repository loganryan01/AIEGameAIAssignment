/*--------------------------------------------------------------------
	File Name: Graph2D.h
	Purpose: Declare functions and macro definitions for source files.
	Author: Logan Ryan
	Modified: 19 August 2020
----------------------------------------------------------------------
	Copyright 2020 Logan Ryan.
--------------------------------------------------------------------*/

#pragma once
#include "Graph.h"

#include <list>
#include <iostream>

#include "raymath.h"

class Graph2D : public Graph<Vector2, float>
{
public:
	// Constructor
	Graph2D();

	// Destructor
	virtual ~Graph2D();

	// Draw the 2D graph
	void Draw();

	// Draw the path
	void DrawPath(std::list<Node*>& path);

	// Set node to be a door node
	void SetDoorNode(float xPos, float yPos);

	// Set node to be a Chaos Emerald node
	void SetChaosNode(float xPos, float yPos);

	// Set node to be a Master Emerald node
	void SetMasterNode(float xPos, float yPos);

	// Set the edges of the node
	void SetEdges(Graph2D::Node* start, float radius);
};

