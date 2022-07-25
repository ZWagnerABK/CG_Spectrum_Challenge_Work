#include "Vehicle.h"
#include <iostream>

Vehicle::Vehicle()
{}

Vehicle::Vehicle(int year, int distance, int time, std::string brand, std::string name)
	: m_year(year), m_distance(distance), m_timeTraveled(time), m_brand(brand), m_name(name)
{}

void Vehicle::Drive(int distance, int time)
{
	m_distance += distance;
	m_timeTraveled += time;
}

int Vehicle::GetYear() const
{
	return m_year;
}

int Vehicle::GetDistance() const
{
	return m_distance;
}

int Vehicle::GetTimeTraveled() const
{
	return m_timeTraveled;
}

std::string Vehicle::GetBrand() const
{
	return m_brand;
}

std::string Vehicle::GetName() const
{
	return m_name;
}

void Vehicle::SetYear(int year)
{
	m_year = year;
}

void Vehicle::SetDistance(int miles)
{
	m_distance = miles;
}

void Vehicle::SetTimeTraveled(int time)
{
	m_timeTraveled = time;
}

void Vehicle::SetBrand(std::string brand)
{
	m_brand = brand;
}

void Vehicle::SetName(std::string name)
{
	m_name = name;
}