#include "Boat.h"
#include <iostream>

Boat::Boat()
{
	CreatedMessage();
}

Boat::Boat(int year, int nauticalMiles, int hoursDriven, std::string brand, std::string personalName, int recommendedMaintenanceHours)
	: Vehicle(year, nauticalMiles, hoursDriven, brand, personalName)
{
	m_maintenanceHours = recommendedMaintenanceHours;
	CreatedMessage();
};

void Boat::Drive(int nauticalMiles, int hours)
{
	Vehicle::Drive(nauticalMiles, hours);

	std::cout << "\nYou drove " << nauticalMiles << " nautical miles in roughly " << hours << " hours in Boat \"" << GetName() << "\".";
}

void Boat::CreatedMessage() const
{
	std::cout << "\nA new Boat has been created!\n";
}

void Boat::DisplayInfo()
{
	std::cout << "\n\nBoat Info\n";
	std::cout << "-----------------------\n";
	std::cout << "Personal Name on Boat: " << GetName() << std::endl;
	std::cout << "Year: " << GetYear() << std::endl;
	std::cout << "Brand: " << GetBrand() << std::endl;
	std::cout << "Milage: " << GetDistance() << " nautical miles" << std::endl;
	std::cout << "Hours Driven: " << GetTimeTraveled() << " hours" << std::endl;
	std::cout << "Recommended Maintenance Hours: " << m_maintenanceHours << " hours" << std::endl;
	std::cout << "Time for maintenance: " << (TimeForMaintenance() ? "Yes" : "No") << std::endl;
}

bool Boat::TimeForMaintenance()
{
	return m_maintenanceHours - GetTimeTraveled() > 0 ? false : true;
}