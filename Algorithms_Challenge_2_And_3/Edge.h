#pragma once
#include <memory>
#include <string>
#include <iostream>


class Edge
{
public:
	Edge() {};
	Edge(char pointID, int cost);

	int GetCost() const { return m_cost; }
	char GetToPointID() const { return m_toPointID; }

	std::string toString();

	~Edge() {};

private:
	char m_toPointID = ' ';
	int m_cost = 0;
};

