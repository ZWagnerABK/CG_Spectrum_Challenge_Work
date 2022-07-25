#pragma once
#include <string>

class Vehicle
{
public:
	Vehicle();
	Vehicle(int year, int distance, int time, std::string brand, std::string name);

	~Vehicle() {};

	int GetYear() const;
	int GetDistance() const;
	int GetTimeTraveled() const;
	std::string GetBrand() const;
	std::string GetName() const;

	void SetYear(int year);
	void SetDistance(int miles);
	void SetTimeTraveled(int time);
	void SetBrand(std::string brand);
	void SetName(std::string name);

	virtual void Drive(int distance, int time);
	virtual void CreatedMessage() const = 0;
	virtual void DisplayInfo() = 0;

private:
	int m_year = 0;
	int m_distance = 0;
	int m_timeTraveled = 0;
	std::string m_brand = "Unspecified";
	std::string m_name = "";
};