#pragma once
#include <vector>
#include <memory>
#include <string>

#include "Path.h"

class Point
{
	public:
		Point() {};
		Point(char ID, int heuristic);
		Point(char ID, std::vector<std::shared_ptr<Path>> paths, int heuristic);

		char GetID() const { return m_ID; }
		int GetNumPaths() const { return m_paths.size(); }
		std::vector<std::shared_ptr<Path>> GetPaths() const { return m_paths; }
		int GetPathValue(char ID);

		std::string toString();

		void InsertPath(std::shared_ptr<Path> path);

		~Point() = default;

	private:
		char m_ID = ' ';
		int m_heuristic = 0;
		std::vector<std::shared_ptr<Path>> m_paths;
};

