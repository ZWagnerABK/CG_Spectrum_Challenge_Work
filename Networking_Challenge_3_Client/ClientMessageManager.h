#pragma once

#include <memory>
#include <functional>
#include <string>

#include "ClientConsoleManager.h"

class ClientMessageManager
{
	public:
		ClientMessageManager();
		~ClientMessageManager() {};

		void SetupChatroomDisplay();
		void SetClientUsername(std::string message, std::function<bool(std::string)> condition, std::string& storage);
		void UserInput(std::string message, std::function<bool(std::string)> condition, std::string& storage);

	private:
		std::unique_ptr<ClientConsoleManager> m_ConsoleManager;
		std::string m_clientUsername;
		const char* m_kWelcomeToChatRoomMessage = "Welcome to the chat room!";
};

