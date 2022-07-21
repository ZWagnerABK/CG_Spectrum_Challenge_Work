#pragma once
#include "Vehicle.h"

class Boat : public Vehicle
{
public:
	Boat();
	Boat(int year, int nauticalMiles, int hoursDriven, std::string brand, std::string personalName, int recommendedMaintenanceHours);

	~Boat() {};

	void Drive(int nauticalMiles, int hours) override;
	void CreatedMessage() override;
	void DisplayInfo() override;

private:
	int m_maintenanceHours = 50;

	bool TimeForMaintenance();
};