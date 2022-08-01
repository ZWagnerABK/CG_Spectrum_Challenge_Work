#pragma once
#include <vector>
#include <memory>
#include <string>

#include "Path.h"

class Point
{
	public:
		Point() {};
		Point(char ID);
		Point(char ID, std::vector<std::shared_ptr<Path>> paths);

		char GetID() const { return m_ID; }

		std::string toString();

		void InsertPath(std::shared_ptr<Path> path);

		~Point() = default;

	private:
		char m_ID = ' ';
		std::vector<std::shared_ptr<Path>> m_paths;
};

