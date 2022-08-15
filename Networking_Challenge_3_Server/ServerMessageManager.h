#pragma once

#include <enet/enet.h>

class ServerMessageManager
{
	public:
		ServerMessageManager() {};
		~ServerMessageManager() {};

		void DisplayServerStartupCompleteMessage();
		void DisplayConnectionMessage(ENetEvent event);
		void DisplayDisconnectMessage(ENetEvent event);
		void DisplayGameStartMessage(int number);
		void DisplayIncorrectGuessMessage(int guessNumber, std::string clientUsername);
		void DisplayCorrectGuessMessage(std::string clientUsername);
		void DisplayNoUsersLeftMessage();
};

