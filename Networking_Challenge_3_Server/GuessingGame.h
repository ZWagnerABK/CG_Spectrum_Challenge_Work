#pragma once
class GuessingGame
{
	public:

		GuessingGame(int lowerLimit, int upperLimit);

		GuessingGame() = delete;
		~GuessingGame() {};

		bool isGuessCorrect(int guess);

	private:
		int m_guessingNumber = 0;
		int m_lowerLimit = 0;
		int m_upperLimit = 0;
};