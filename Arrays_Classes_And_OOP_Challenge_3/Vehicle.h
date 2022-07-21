#pragma once
#include <string>
#include <iostream>

class Vehicle
{
public:
	Vehicle() {};
	Vehicle(int year, int distance, int time, std::string brand, std::string name)
		:m_year(year), m_distance(distance), m_timeTraveled(time), m_brand(brand), m_name(name)
	{};

	~Vehicle() {};

	int GetYear() const { return m_year; } ;
	int GetDistance() const { return m_distance; };
	int GetTimeTraveled() const { return m_timeTraveled; };
	std::string GetBrand() const { return m_brand; };
	std::string GetName() const { return m_name; };

	void SetYear(int year) { m_year = year; };
	void SetDistance(int miles) { m_distance = miles; };
	void SetTimeTraveled(int time) { m_timeTraveled = time; };
	void SetBrand(std::string brand) { m_brand = brand; };
	void SetName(std::string name) { m_name = name; };

	virtual void Drive(int distance, int time);
	virtual void CreatedMessage() = 0;
	virtual void DisplayInfo() = 0;

private:
	int m_year = 0;
	int m_distance = 0;
	int m_timeTraveled = 0;
	std::string m_brand = "Unspecified";
	std::string m_name = "";
};