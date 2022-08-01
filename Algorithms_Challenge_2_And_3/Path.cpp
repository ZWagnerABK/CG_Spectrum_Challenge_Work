#include "Path.h"

Path::Path(char pointID, int cost)
{
	m_toPointID = pointID;
	m_cost = cost;
}

std::string Path::toString() 
{
	std::string display = "";

	display += "To Point: ";
	display += m_toPointID;
	display += "\n";

	display += "Cost: ";
	display += std::to_string(m_cost);
	display += "\n";

	return display;
}