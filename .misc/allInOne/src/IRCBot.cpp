
#include "IRCBot.hpp"
#include <ctime>	// For time(NULL) to fetch the current time

/*
Constructs an IRCBot with the specified nickname.
The nickname is used to identify the bot when interacting with clients.

Parameters:
	nick - The nickname for the bot.
*/
IRCBot::IRCBot(const std::string& nick) : nickname(nick) {}

/*
Destroys the IRCBot instance.
Currently, no special cleanup is required.
*/
IRCBot::~IRCBot(void) {}

/*
Processes a command and generates an appropriate response.

Parameters:
	command - The command received by the bot.
Returns:
	A string containing the bot's response to the command.

Supported commands:
	- HELLO: Responds with a greeting including the bot's nickname.
	- TIME: Responds with the current system time.
	- Any other command: Responds with an "unknown command" message.
*/
std::string	IRCBot::handleCommand(const std::string &command)
{
	if (command == "HELLO")
	{
		// Respond with a friendly greeting
		return ("Hello! I'm " + nickname + ", your friendly bot.");
	}
	else if (command == "TIME")
	{
		// Respond with the current system time (as a UNIX timestamp)
		return ("The current time is: " + std::to_string(time(NULL)));
	}
	else
	{
		// Respond with a message indicating the command is unrecognized
		return ("I don't understand the command: " + command);
	}
}

// implementation in IRCServer::processCommand

if (cmd == "BOT")
{
	// BOT <command>
	// Initialize a bot with a specific nickname
	IRCBot	bot("HelperBot");

	// Process the command using the bot
	std::string	response = bot.handleCommand(arg1);

	// Send the bot's response back to the client
	NetworkUtils::sendData(client->getSocket(), response + "\r\n");
}

/*
Handles the "BOT" command.
	BOT <command>
Interacts with a bot instance to process a specific command and sends the
bot's response back to the client.

Steps:
1. Instantiate an IRCBot with a predefined nickname (e.g., "HelperBot").
2. Use the bot's `handleCommand` method to process the provided command.
3. Send the bot's response back to the client.

Examples:
	BOT HELLO -> "Hello! I'm HelperBot, your friendly bot."
	BOT TIME  -> "The current time is: <timestamp>"
	BOT XYZ   -> "I don't understand the command: XYZ"
*/