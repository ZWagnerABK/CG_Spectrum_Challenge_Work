#include <iostream>
#include <memory>
#include <vector>
#include "Path.h"
#include "Point.h"

int main()
{
    std::cout << "Hello!  This program will attempt to run an implementation of the A* Algorithm using a pre-determined set of coordinates.\n";

    std::shared_ptr<Path> path1 = std::make_shared<Path>('B', 5);
    std::shared_ptr<Path> path2 = std::make_shared<Path>('C', 1);
    std::shared_ptr<Path> path3 = std::make_shared<Path>('D', 7);
    std::shared_ptr<Path> path4 = std::make_shared<Path>('D', 1);

    std::unique_ptr<Point> aPoint = std::make_unique<Point>('A');
    aPoint.get()->InsertPath(path1);
    aPoint.get()->InsertPath(path3);

    std::unique_ptr<Point> bPoint = std::make_unique<Point>('B');
    bPoint.get()->InsertPath(path2);

    std::unique_ptr<Point> cPoint = std::make_unique<Point>('C');
    cPoint.get()->InsertPath(path4);

    std::unique_ptr<Point> dPoint = std::make_unique<Point>('D');

    std::cout << aPoint.get()->toString() << std::endl;
    std::cout << bPoint.get()->toString() << std::endl;
    std::cout << cPoint.get()->toString() << std::endl;
    std::cout << dPoint.get()->toString() << std::endl;
}
