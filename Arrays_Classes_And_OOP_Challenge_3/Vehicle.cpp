#include "Vehicle.h"

void Vehicle::Drive(int distance, int time)
{
	m_distance += distance;
	m_timeTraveled += time;
}