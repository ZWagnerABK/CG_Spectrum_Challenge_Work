#pragma once

/*
* Common is a static library being used by Networking_Challenge_3_Client and Networking_Challenge_3_Server to share packet structs, resource strings, and an enum representing packet headers.
*/

#include <string>

namespace common {

	enum class PacketHeaderTypes: unsigned
	{
		PHT_INVALID = 0,
		PHT_ID,
		PHT_START,
		PHT_GUESS,
		PHT_GUESS_RESULT
	};

	struct GamePacket
	{
		PacketHeaderTypes Type = PacketHeaderTypes::PHT_INVALID;
	};

	struct IDPacket : GamePacket
	{
		int id;
	};

	struct StartPacket: GamePacket
	{
		bool start;
	};

	struct GuessPacket : GamePacket
	{
		int guess;
		int clientID;
		std::string userName;
	};

	struct GuessResultPacket : GamePacket
	{
		bool guessedRight;
		int guess;
		int clientID;
		std::string clientUsername;
	};
}

namespace resources
{
	static const char* kServerStartupCompleteMessage = "Server startup complete! Waiting for clients to connect. . .\n";
	static const char* kErrorWhileInitializingENETMessage = "An error occurred while initializing ENet.\n";
	static const char* kErrorWhileCreatingENetServerHostMessage = "An error occurred while trying to create an ENet server host.\n";
	static const char* kENetDefaultErrorMessage = "An error occurred while trying to create an ENet client host.\n";
	static const char* kENetInitializingErrorMessage = "An error occurred while initializing ENet.\n";
	static const char* kENetNoPeerAvialable = "No available peers for initiating an ENet connection.\n";
	static const char* kEnterUserName = "Enter a name for the client user: ";
	static const char* kInvalidPacketMessage = "Invalid Packet.";
	static const char* kENetConnectionSuccess = "Connection to 127.0.0.1:1234 succeeded.  Waiting for other player to join.";
	static const char* kENetConnectionFailure = "Connection 127.0.0.1:1234 failed.";
	static const char* kGameStartingMessage = "Players have connected!  Game is now starting.  The number to guess is ";
	static const char* kInvalidNumberOfClientsMessage = "Invalid number of clients have connected.  Shutting down the game.";
	static const char* kWelcomeMessage = "Welcome to the number guessing game!";
	static const char* kInputLine = "Enter a number to guess >> ";
	static const char* kIncorrectGuessServerMessage = " entered an incorrect guess of ";
	static const char* kIncorrectGuessClientMessage = " guessed wrong with a guess of ";
	static const char* kYouIncorrectGuessClientMessage = "You guessed wrong with a guess of ";
	static const char* kCorrectGuessServerMessage = " guessed correctly!! Ending game for ";
	static const char* kCorrectGuessClientMessage = " guessed correctly!!";
	static const char* kYouCorrectGuessClientMessage = "You guessed correctly!! The correct number was ";
	static const char* kEndingClientMessage = "Ending game.";
	static const char* kNoClientsRemain = "No clients remain. Shutting down server.";
}