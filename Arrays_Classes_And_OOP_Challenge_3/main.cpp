#include <iostream>
#include "Car.h"
#include "Boat.h"
#include "Airplane.h"

int main()
{
    Car* newCar = new Car(2020, 0, 0, "Toyota", "My Car", 3000);
    newCar->Drive(100, 2);
    newCar->DisplayInfo();

    delete newCar;

    Boat* newBoat = new Boat(1995, 900, 98, "G3", "My Fishing Boat", 100);
    newBoat->Drive(105, 3);
    newBoat->DisplayInfo();

    delete newBoat;

    Airplane* newAirplane = new Airplane(2022, 300, 2, "Boeing", "#1342", 50);
    newAirplane->Drive(5834, 7);
    newAirplane->DisplayInfo();

    delete newAirplane;
}