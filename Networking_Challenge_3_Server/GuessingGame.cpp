#include "GuessingGame.h"

#include <iostream>

GuessingGame::GuessingGame(int lowerLimit, int upperLimit)
{
	m_lowerLimit = lowerLimit;
	m_upperLimit = upperLimit;

	srand(time(NULL));
	m_guessingNumber = (rand() + lowerLimit) % upperLimit;
}

bool GuessingGame::isGuessCorrect(int guess)
{
	if (guess == m_guessingNumber)
	{
		return true;
	}

	return false;
}