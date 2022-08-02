#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <queue>
#include <tuple>

#include "Edge.h"
#include "Point.h"

void AStarAlgo(std::map<char, std::shared_ptr<Point>>& graph, char root, char destination);
std::string ReconstructEdge(std::map<char, std::shared_ptr<Point>>& cameFrom, char root, char destination);

struct PriorityQueueItem
{
    int cost;
    std::shared_ptr<Point> location;
};

struct PriorityQueueItemComparator
{
    bool operator ()(const PriorityQueueItem& left, const PriorityQueueItem& right)
    {
        return left.cost > right.cost;
    }
};

int main()
{
    std::cout << "Hello!  This program will attempt to run an implementation of the A* Algorithm using a pre-determined set of coordinates.\n";

    std::shared_ptr<Edge> path1 = std::make_shared<Edge>('B', 5);
    std::shared_ptr<Edge> path2 = std::make_shared<Edge>('C', 1);
    std::shared_ptr<Edge> path3 = std::make_shared<Edge>('D', 8);
    std::shared_ptr<Edge> path4 = std::make_shared<Edge>('D', 1);

    std::shared_ptr<Point> aPoint = std::make_shared<Point>('A', 0);
    aPoint.get()->InsertEdge(path1);
    aPoint.get()->InsertEdge(path3);

    std::shared_ptr<Point> bPoint = std::make_shared<Point>('B', 0);
    bPoint.get()->InsertEdge(path2);

    std::shared_ptr<Point> cPoint = std::make_shared<Point>('C', 0);
    cPoint.get()->InsertEdge(path4);

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
        priorityQueue.pop();

        std::shared_ptr<Point> location = priorityQueueItem.location;
        int locationCost = priorityQueueItem.cost;
        
        //If you reach your destination, leave the loop
        if (location.get()->GetID() == destination)
            break;

        int size = location.get()->GetNumEdges();

        //Loop through the current point's neighbors through its paths
        for (int i = 0; i < size; i++)
        {
            auto currentEdge = location.get()->GetEdge(i);
            char nextID = currentEdge.get()->GetToPointID();
            int cost = currentEdge.get()->GetCost();

            //Add cost of the current location with the cost of the current neighbor
            int newCost = costSoFar[location.get()->GetID()] + cost;

            if (costSoFar.find(nextID) == costSoFar.end() || newCost < costSoFar[nextID])
            {
                costSoFar[nextID] = newCost;
                priorityQueue.push({ newCost, graph.at(nextID) });
                cameFrom[nextID] = location;
            }
        }
    }

    std::cout << ReconstructEdge(cameFrom, root, destination) << std::endl;
}

std::string ReconstructEdge(std::map<char, std::shared_ptr<Point>>& cameFrom, char root, char destination)
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
        cost += cameFrom[currentLocation]->GetEdgeValue(currentLocation);
        currentLocation = nextLocation;
    }

    path.append(1, root);
    std::reverse(path.begin(), path.end());

    path.append("\n");
    path.append("Cost: " + std::to_string(cost));

    return path;
}