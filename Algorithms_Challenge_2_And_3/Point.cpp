#include "Point.h"

Point::Point(char ID)
{
	m_ID = ID;
}

Point::Point(char ID, std::vector<std::shared_ptr<Path>> paths)
{
	m_ID = ID;
	m_paths = paths;
}

void Point::InsertPath(std::shared_ptr<Path> path)
{
	m_paths.push_back(std::move(path));
}

std::string Point::toString()
{
	std::string display = "";

	display += "Point ID: ";
	display += m_ID;
	display += "\n";
	display += "---------------\n";

	if (m_paths.size() == 0)
	{
		display += "No paths originating from this point.\n";
		display += "---------------\n";
	}
	else
	{
		for (int i = 0; i < m_paths.size(); i++)
		{
			display += "Path " + std::to_string(i + 1) + ":\n";
			display += m_paths[i].get()->toString();
			display += "---------------\n";
		}
	}

	return display;
}