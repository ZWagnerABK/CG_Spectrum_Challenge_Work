#pragma once
#include <string>

class Car
{
	//Member variables
	int m_year;
	int m_miles;
	std::string m_brand;

public:
	// Constructors
	Car();
	Car(int year, int miles, std::string brand);

	// Destructor
	~Car();

	// Member functions
	void Display();
	void Drive(int miles);
};