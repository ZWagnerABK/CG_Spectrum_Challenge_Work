/*
* ClientConsole used as an internal class by ClientMessageManager mainly to seperate out the functionaliy directly interacting with the console from the actual messages being displayed.
*/

#pragma once
#include <functional>
#include <Windows.h>

class ClientConsole
{
	public:
		ClientConsole() {};
		~ClientConsole() {};

		void SetInputMessageLength(int length);
		void ResetConsole();
		void EraseConsoleLine();
		void RepositionInputCursor(bool inital);
		void RepositionToLogPostion();
		void IncremenetLogPosition();
		void SetupConsole();

	private:
		//Used to reposition the cursor [See RepositionInputCursor]
		int m_inputMessageLength = 0;

		short m_logStartXPos = 0;
		short m_logStartYPos = 3;
		short m_currentLogXPos = 0;
		short m_currentLogYPos = 0;
};

