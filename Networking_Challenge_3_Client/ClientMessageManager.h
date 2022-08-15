/*
* ClientMessageManager is used as a way of managing the messages that will be displayed on the console screen.
*/

#pragma once

#include <memory>
#include <functional>
#include <string>
#include <iostream>
#include "ClientConsole.h"

class ClientMessageManager
{
	public:
		ClientMessageManager();
		~ClientMessageManager() {};

		void SetupGuessingRoomDisplay();
		void SetClientUsername(std::string message, std::function<bool(std::string)> condition, std::string& storage);

		void UserInput(std::string message, std::function<bool(std::string)> condition, std::string& storage);
		void UserInput(std::string message, std::function<bool(int)> condition, int& storage);

		void DisplayWrongGuessMessage(bool didIGuessWrong, std::string otherUsername, int wrongGuess);
		void DisplayCorrectGuessMessage(bool didIGuessWrong, std::string otherUsername, int wrongGuess);

		void SetupConsole();

	private:
		//A internal class to seperate out code that interacts with the console.
		std::unique_ptr<ClientConsole> m_Console;
		std::string m_clientUsername;
};

