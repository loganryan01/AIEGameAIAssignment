#pragma once
#include "Graph.h"

#include <list>
#include <iostream>

#include "raymath.h"

class Graph2D : public Graph<Vector2, float>
{
public:
	Graph2D();
	virtual ~Graph2D();

	void Draw();

	void SetDoorNode(float xPos, float yPos);

	void GetNearbyNodes(Vector2 position, float radius, std::vector<Graph2D::Node*>& out_nodes);
};

