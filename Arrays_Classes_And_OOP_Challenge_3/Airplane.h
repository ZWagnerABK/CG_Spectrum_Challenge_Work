#pragma once
#include "Vehicle.h"

class Airplane : public Vehicle
{
public:
	Airplane();
	Airplane(int year, int kilometers, int time, std::string brand, std::string personalName, int recommendedMaintenanceHours);
		
	~Airplane() {};

	void Drive(int kilometers, int hours) override;
	void CreatedMessage() const override;
	void DisplayInfo() override;

private:
	int m_maintenanceHours = 100;

	bool TimeForMaintenance();
};