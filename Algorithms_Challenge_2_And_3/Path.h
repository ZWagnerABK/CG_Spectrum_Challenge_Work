#pragma once
#include <memory>
#include <string>
#include <iostream>


class Path
{
public:
	Path() {};
	Path(char pointID, int cost);

	std::string toString();

	~Path() {};

private:
	char m_toPointID = ' ';
	int m_cost = 0;
};

