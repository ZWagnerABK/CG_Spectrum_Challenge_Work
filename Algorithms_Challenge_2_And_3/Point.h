#pragma once
#include <vector>
#include <memory>
#include <string>

#include "Edge.h"

class Point
{
	public:
		Point() {};
		Point(char ID, int heuristic);
		Point(char ID, std::vector<std::shared_ptr<Edge>> paths, int heuristic);

		char GetID() const { return m_ID; }
		int GetNumEdges() const { return m_edges.size(); }
		std::vector<std::shared_ptr<Edge>> GetEdges() const { return m_edges; }
		int GetEdgeValue(char ID);

		std::shared_ptr<Edge> GetEdge(int index) const;

		std::string toString();

		void InsertEdge(std::shared_ptr<Edge> edge);

		~Point() = default;

	private:
		char m_ID = ' ';
		int m_heuristic = 0;
		std::vector<std::shared_ptr<Edge>> m_edges;
};

