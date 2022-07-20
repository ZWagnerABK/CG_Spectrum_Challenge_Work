#include "Car.h"
#include <iostream>

Car::Car() : m_year(2019), m_miles(0), m_brand("Unspecified")
{}

Car::Car(int year, int miles, std::string brand)
	:	m_year(year),
		m_miles(miles),
		m_brand(brand)
{}

Car::~Car()
{

}

void Car::Display() 
{
	std::cout << "Brand: " << m_brand << std::endl;
	std::cout << "Year: " << m_year << std::endl;
	std::cout << "Miles: " << m_miles << std::endl;
}

void Car::Drive(int miles)
{
	m_miles = m_miles + miles;
}