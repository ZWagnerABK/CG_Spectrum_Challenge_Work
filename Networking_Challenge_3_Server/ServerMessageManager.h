#pragma once

#include <enet/enet.h>

class ServerMessageManager
{
	public:
		ServerMessageManager() {};
		~ServerMessageManager() {};

		void DisplayConnectionMessage(ENetEvent event);
		void DisplayDisconnectMessage(ENetEvent event);

	private:

};

