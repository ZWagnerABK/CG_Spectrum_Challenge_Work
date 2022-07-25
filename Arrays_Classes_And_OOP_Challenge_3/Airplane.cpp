#include "Airplane.h"
#include <iostream>

Airplane::Airplane()
{
	CreatedMessage();
}

Airplane::Airplane(int year, int kilometers, int hoursFlown, std::string brand, std::string personalName, int recommendedMaintenanceHours)
	: Vehicle(year, kilometers, hoursFlown, brand, personalName) 
{
	m_maintenanceHours = recommendedMaintenanceHours;
	CreatedMessage();
}

void Airplane::Drive(int kilometers, int hours)
{
	Vehicle::Drive(kilometers, hours);

	std::cout << "\nYou flew " << kilometers << " kilometers in roughly " << hours << " hours in Airplane \"" << GetName() << "\".";
}

void Airplane::DisplayInfo()
{
	std::cout << "\n\nAirplane Info\n";
	std::cout << "-----------------------\n";
	std::cout << "Personal Name on Plane: " << GetName() << std::endl;
	std::cout << "Year: " << GetYear() << std::endl;
	std::cout << "Brand: " << GetBrand() << std::endl;
	std::cout << "Kilometers: " << GetDistance() << " kilometers" << std::endl;
	std::cout << "Hours Flown: " << GetTimeTraveled() << " hours" << std::endl;
	std::cout << "Recommended Maintenance Hours: " << m_maintenanceHours << " hours" << std::endl;
	std::cout << "Time for maintenance: " << (TimeForMaintenance() ? "Yes" : "No") << std::endl;
}

void Airplane::CreatedMessage() const 
{
	std::cout << "\nA new Airplane has been created!\n";
}

bool Airplane::TimeForMaintenance() 
{
	return m_maintenanceHours - GetTimeTraveled() > 0 ? false : true;
}