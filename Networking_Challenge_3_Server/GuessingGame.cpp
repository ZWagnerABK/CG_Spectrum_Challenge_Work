#include "GuessingGame.h"

#include <iostream>

GuessingGame::GuessingGame(int lowerLimit, int upperLimit)
{
	m_lowerLimit = lowerLimit;
	m_upperLimit = upperLimit;

	srand(time(NULL));
	m_guessingNumber = (rand() + lowerLimit) % upperLimit;
}

int GuessingGame::GetGuessingNumber()
{
	return m_guessingNumber;
}

bool GuessingGame::IsGuessCorrect(int guess)
{
	if (guess == m_guessingNumber)
	{
		return true;
	}

	return false;
}