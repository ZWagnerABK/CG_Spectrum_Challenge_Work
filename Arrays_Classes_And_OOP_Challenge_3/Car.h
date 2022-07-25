#pragma once
#include "Vehicle.h"

class Car : public Vehicle
{
public:
	Car();
	Car(int year, int miles, int hoursDriven, std::string brand, std::string personalName, int oilChangeMilage);

	~Car() {};

	int GetOilChangeMilage() const { return m_oilChangeMilage; };

	void SetOilChangeMilage(int oilChangeMilage) { m_oilChangeMilage = oilChangeMilage; };

	void Drive(int miles, int hours) override;
	void CreatedMessage() const override;
	void DisplayInfo() override;

private:
	int m_oilChangeMilage = 3000;

	bool TimeForOilChange();
};