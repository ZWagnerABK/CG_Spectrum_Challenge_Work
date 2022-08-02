#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <queue>
#include <tuple>

#include "Path.h"
#include "Point.h"

void AStarAlgo(std::map<char, std::shared_ptr<Point>>& graph, char root, char destination);
std::string ReconstructPath(std::map<char, std::shared_ptr<Point>>& cameFrom, char root, char destination);

struct PriorityQueueItem
{
    int cost;
    std::shared_ptr<Point> location;
};

struct PriorityQueueItemComparator
{
    bool operator()(const PriorityQueueItem& left, const PriorityQueueItem& right)
    {
        return left.cost > right.cost;
    }
};

int main()
{
    std::cout << "Hello!  This program will attempt to run an implementation of the A* Algorithm using a pre-determined set of coordinates.\n";

    std::shared_ptr<Path> path1 = std::make_shared<Path>('B', 5);
    std::shared_ptr<Path> path2 = std::make_shared<Path>('C', 1);
    std::shared_ptr<Path> path3 = std::make_shared<Path>('D', 7);
    std::shared_ptr<Path> path4 = std::make_shared<Path>('D', 1);

    std::shared_ptr<Point> aPoint = std::make_shared<Point>('A', 0);
    aPoint.get()->InsertPath(path1);
    aPoint.get()->InsertPath(path3);

    std::shared_ptr<Point> bPoint = std::make_shared<Point>('B', 0);
    bPoint.get()->InsertPath(path2);

    std::shared_ptr<Point> cPoint = std::make_shared<Point>('C', 0);
    cPoint.get()->InsertPath(path4);

    std::shared_ptr<Point> dPoint = std::make_shared<Point>('D', 0);

    std::map<char, std::shared_ptr<Point>> graph = {
        {'A', aPoint}, {'B', bPoint}, {'C', cPoint}, {'D', dPoint}
    };

    std::cout << aPoint.get()->toString() << std::endl;
    std::cout << bPoint.get()->toString() << std::endl;
    std::cout << cPoint.get()->toString() << std::endl;
    std::cout << dPoint.get()->toString() << std::endl;

    AStarAlgo(graph, 'A', 'D');
}


//NOT CURRENT A*.  NEED TO ADD THAT IN NEXT 
void AStarAlgo(std::map<char, std::shared_ptr<Point>>& graph, char root, char destination)
{
    std::priority_queue<PriorityQueueItem, std::vector<PriorityQueueItem>, PriorityQueueItemComparator> priorityQueue;

    priorityQueue.push({ 0, std::move(graph.at(root)) });

    //Came from begins empty
    std::map<char, std::shared_ptr<Point>> cameFrom;
    //Cost so far begins at 0 at the starting location (this case A)
    std::map<char, int> costSoFar = { {root, 0} };

    while (!priorityQueue.empty())
    {
        auto priorityQueueItem = priorityQueue.top();
        std::shared_ptr<Point> location = priorityQueueItem.location;
        int locationCost = priorityQueueItem.cost;
        
        //If you reach your destination, leave the loop
        if (location.get()->GetID() == destination)
            break;

        int size = location.get()->GetNumPaths();
        auto paths = location.get()->GetPaths();

        //Loop through the current point's neighbors through its paths
        for (int i = 0; i < size; i++)
        {
            char nextID = paths[i]->GetToPointID();

            //Add cost of the current location with the cost of the current neighbor
            int newCost = costSoFar[location.get()->GetID()] + paths[i]->GetCost();

            if (costSoFar.find(nextID) == costSoFar.end() || newCost < costSoFar[nextID])
            {
                costSoFar[paths[i]->GetToPointID()] = newCost;
                priorityQueue.push({ newCost, graph.at(nextID) });
                cameFrom[nextID] = location;
            }
        }

        priorityQueue.pop();
    }

    std::cout << ReconstructPath(cameFrom, root, destination) << std::endl;
}

std::string ReconstructPath(std::map<char, std::shared_ptr<Point>>& cameFrom, char root, char destination)
{

    char currentLocation = destination;
    char nextLocation = ' ';
    std::string path = "";
    int cost = 0;

    while (currentLocation != root)
    {
        path.append(1, currentLocation);
        path += " ";

        nextLocation = cameFrom[currentLocation]->GetID();
        cost += cameFrom[currentLocation]->GetPathValue(currentLocation);
        currentLocation = nextLocation;
    }

    path.append(1, root);
    std::reverse(path.begin(), path.end());

    path.append("\n");
    path.append("Cost: " + std::to_string(cost));

    return path;
}