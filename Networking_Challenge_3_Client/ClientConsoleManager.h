#pragma once
class ClientConsoleManager
{
	public:
		ClientConsoleManager() {};
		~ClientConsoleManager() {};

		void SetClientUsernameLength(int length);
		void ResetConsole();
		void EraseConsoleLine();
		void RepositionInputCursor(bool inital);

	private:
		int m_clientUsernameLength = 0;
		short m_logStartXPos = 0;
		short m_logStartYPos = 7;
		short m_currentLogXPos = 0;
		short m_currentLogYPos = 0;
};

