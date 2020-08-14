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
		else if (node->chaosNode == true)
		{
			DrawCircle(node->data.x, node->data.y, 8, BLUE);
			DrawCircleLines(node->data.x, node->data.y, 8, DARKBLUE);
		}
		else if (node->masterNode == true)
		{
			DrawCircle(node->data.x, node->data.y, 8, BROWN);
			DrawCircleLines(node->data.x, node->data.y, 8, DARKBROWN);
		}
		else
		{
			DrawCircle(node->data.x, node->data.y, 8, GRAY);
			DrawCircleLines(node->data.x, node->data.y, 8, LIGHTGRAY);
		}
	}

	// Draw all connections
	/*for (auto node : GetNodes())
	{
		for (auto connection : node->connections)
		{
			DrawLine(node->data.x, node->data.y, connection.to->data.x, connection.to->data.y, RED);
		}
	}*/
}

void Graph2D::DrawPath(std::list<Node*>& path)
{
	for (auto node : path)
	{
		for (auto connection : node->connections)
		{
			DrawLine(node->data.x, node->data.y, connection.to->data.x, connection.to->data.y, RED);
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

void Graph2D::SetChaosNode(float xPos, float yPos)
{
	Node* node = new Node();
	node->data = { xPos, yPos };
	node->chaosNode = true;

	m_nodes.push_back(node);
}

void Graph2D::SetMasterNode(float xPos, float yPos)
{
	Node* node = new Node();
	node->data = { xPos, yPos };
	node->masterNode = true;

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