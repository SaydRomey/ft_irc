
#ifndef IRCBOT_HPP
#define IRCBOT_HPP

#include <string>
#include "IRCClient.hpp"

/*
The IRCBot class represents a simple bot that
listens to specific commands and responds with predefined messages.

It can be extended to include more complex behaviors as needed.
*/
class IRCBot
{
	private:
		std::string	nickname;	// The bot's nickname, used for identification

	public:
		IRCBot(const std::string &nick);
		~IRCBot(void);

		std::string	handleCommand(const std::string &command);
};

#endif // IRCBOT_HPP
