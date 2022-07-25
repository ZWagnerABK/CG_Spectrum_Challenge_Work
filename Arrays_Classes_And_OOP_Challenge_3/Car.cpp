#include "Car.h"
#include <iostream>

Car::Car()
{
	CreatedMessage();
}

Car::Car(int year, int miles, int hoursDriven, std::string brand, std::string personalName, int oilChangeMilage)
	: Vehicle(year, miles, hoursDriven, brand, personalName)
{
	m_oilChangeMilage = oilChangeMilage;
	CreatedMessage();
};

void Car::Drive(int miles, int hours)
{
	Vehicle::Drive(miles, hours);

	std::cout << "\nYou drove " << miles << " miles in roughly " << hours << " hours in Car \"" << GetName() << "\".";
}

void Car::CreatedMessage() const
{
	std::cout << "\nA new Car has been created!\n";
}

void Car::DisplayInfo()
{
	std::cout << "\n\nCar Info\n";
	std::cout << "-----------------------\n";
	std::cout << "Personal Name on Car: " << GetName() << std::endl;
	std::cout << "Year: " << GetYear() << std::endl;
	std::cout << "Brand: " << GetBrand() << std::endl;
	std::cout << "Milage: " << GetDistance() << " miles" << std::endl;
	std::cout << "Hours Driven: " << GetTimeTraveled() << " hours" << std::endl;
	std::cout << "Oil Change Milage on Sticker: " << m_oilChangeMilage << " miles" << std::endl;
	std::cout << "Time for next oil change: " << (TimeForOilChange() ? "Yes" : "No") << std::endl;
}

bool Car::TimeForOilChange()
{
	return m_oilChangeMilage - GetDistance() > 0 ? false : true;
}