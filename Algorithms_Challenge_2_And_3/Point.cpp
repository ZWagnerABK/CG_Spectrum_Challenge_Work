#include "Point.h"

Point::Point(char ID, int heuristic)
{
	m_ID = ID;
	m_heuristic = heuristic;
}

Point::Point(char ID, std::vector<std::shared_ptr<Edge>> edges, int heuristic)
{
	m_ID = ID;
	m_edges = edges;
	m_heuristic = heuristic;
}

void Point::InsertEdge(std::shared_ptr<Edge> path)
{
	m_edges.push_back(path);
}

int Point::GetEdgeValue(char ID)
{
	for (int i = 0; i < m_edges.size(); i++)
	{
		if (m_edges[i]->GetToPointID() == ID)
		{
			return m_edges[i]->GetCost();
		}
	}

	return -1;
}

std::shared_ptr<Edge> Point::GetEdge(int index) const
{
	std::shared_ptr<Edge> edge;

	if (index < m_edges.size() && index >= 0)
	{
		edge = m_edges[index];
	}

	return edge;
}

std::string Point::toString()
{
	std::string display = "";

	display += "Point ID: ";
	display += m_ID;
	display += "\n";
	display += "---------------\n";

	if (m_edges.size() == 0)
	{
		display += "No edges originating from this point.\n";
		display += "---------------\n";
	}
	else
	{
		for (int i = 0; i < m_edges.size(); i++)
		{
			display += "Edge " + std::to_string(i + 1) + ":\n";
			display += m_edges[i].get()->toString();
			display += "---------------\n";
		}
	}

	return display;
}