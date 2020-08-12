#include "Graph2D.h"

Graph2D::Graph2D()
{

}

Graph2D::~Graph2D()
{

}

void Graph2D::Draw()
{
	// Draw all nodes
	for (auto node : GetNodes())
	{
		if (node->doorNode == true)
		{
			DrawCircle(node->data.x, node->data.y, 8, GREEN);
			DrawCircleLines(node->data.x, node->data.y, 8, DARKGREEN);
		}
		else
		{
			DrawCircle(node->data.x, node->data.y, 8, GRAY);
			DrawCircleLines(node->data.x, node->data.y, 8, LIGHTGRAY);
		}
	}
}

void Graph2D::SetDoorNode(float xPos, float yPos)
{
	Node* node = new Node();
	node->data = { xPos, yPos };
	node->doorNode = true;

	m_nodes.push_back(node);
}

void Graph2D::GetNearbyNodes(Vector2 position, float radius, std::vector<Graph2D::Node*>& out_nodes)
{
	for (auto node : m_nodes)
	{
		float dist = Vector2Distance(position, node->data);
		if (dist < radius)
		{
			out_nodes.push_back(node);
		}
	}
}