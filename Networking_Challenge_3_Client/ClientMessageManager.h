#pragma once

#include <memory>
#include <functional>
#include <string>
#include <format>
#include <string_view>
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

	private:
		std::unique_ptr<ClientConsole> m_Console;
		std::string m_clientUsername;
};

