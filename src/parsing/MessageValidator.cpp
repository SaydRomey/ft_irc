/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageValidator.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:48:44 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/18 15:57:34 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageValidator.hpp"
#include <cctype>
#include <set>
#include <iostream> // for debug

/*
Commands to Implement

Authentication Commands:

PASS: Client provides the connection password.
NICK: Client sets their nickname.
USER: Client sets their username.

Channel Management Commands:

JOIN: Join a specific channel.
PART: Leave a specific channel.
TOPIC: View or change the channel's topic.
MODE: Change channel modes (i, t, k, o, l).
KICK: Eject a client from a channel.
INVITE: Invite a client to a channel.

Messaging Commands:

PRIVMSG: Send a private message to a user or channel.
NOTICE: Send a notice to a user or channel.

*/

/*
must be able to:
authenticate "PASS"
set nickname "NICK"
set username "USER"
join a channel "JOIN"
send and recieve private messages "PRIVMSG"

implement commands specific to channel operators:
KICK
INVITE
TOPIC
MODE
 i
 t
 k
 o
 l

{"PASS", "NICK", "USER", "JOIN", "PRIVMSG",
 "KICK", "INVITE", "TOPIC", "MODE",
 "PART", "NOTICE", "PING", "PONG", "QUIT"}; // last row not needed..
*/

MessageValidator::MessageValidator(void) {}

bool	MessageValidator::isValidCommand(const std::map<std::string, std::string> &command)
{
	static const std::string	commandsArray[] = {\
	"PASS", "NICK", "USER", "JOIN", "PRIVMSG", \
	"KICK", "INVITE", "TOPIC", "MODE", \
	"PART", "NOTICE", "PING", "PONG", "QUIT"}; // last row not needed..
	static const std::set<std::string>	validCommands(commandsArray, commandsArray + sizeof(commandsArray) / sizeof(commandsArray[0]));
	
	if (!command.count("command") || command.at("command").empty())
	{
		std::cout << "** Debug: Command field is missing or empty." << std::endl;
		return (false);
	}
	
	const std::string	&cmd = command.at("command");

	// Allow numeric replies
	if (cmd.size() == 3 && std::isdigit(cmd[0]) && std::isdigit(cmd[1]) && std::isdigit(cmd[2]))
	{
		std::cout << "** Debug: Command is a valid numeric reply: " << cmd << std::endl;
		return (true);
	}

	if (validCommands.find(cmd) == validCommands.end())
	{
		std::cout << "** Debug: Command is not a valid IRC command: " << cmd << std::endl;
		return (false);
	}
	
	std::cout << "** Debug: Command is valid: " << cmd << std::endl;
	return (true);
}

bool	MessageValidator::isValidNickname(const std::string &nickname)
{
	if (nickname.empty() || !std::isalpha(nickname[0]))
		return (false); // must start with a letter
	
	size_t	i = 1;
	while (i < nickname.size())
	{
		if (!std::isalnum(nickname[i]) && nickname[i] != '-' && nickname[i] != '_')
		{
			return (false); // only allow letters, digits, dashes and underscores
		}
		++i;
	}
	return (true);
}

bool	MessageValidator::isValidChannel(const std::string &channel)
{
	return (!channel.empty() && channel[0] == '#' && _isAlphanumeric(channel.substr(1)));
}

/* untested new functions..

*/
bool	MessageValidator::isValidTrailing(const std::string &trailing)
{
	return (!trailing.empty()); // basic check  for now..
}

bool	MessageValidator::isValidModeCommand(const std::vector<std::string> &params)
{
	if (params.size() < 2)
		return (false);
	
	const std::string &channel = params[0];
	return (isValidChannel(channel));
}


// bool	MessageValidator::isValidPrivmsg(const std::vector<std::string> &params);

// bool	MessageValidator::isChannelOperatorCommand(const std::string &command)
// {
// 	static const std::set<std::string>	operatorCommands = {"KICK", "INVITE", "TOPIC", "MODE"};
	
// 	return (operatorCommands.find(command) != operatorCommands.end());
// }

// bool	MessageValidator::isMessagingCommand(const std::string &command)
// {
// 	static const std::set<std::string>	messagingCommands = {"PRIVMSG", "NOTICE"};
	
// 	return (messagingCommands.find(command) != messagingCommands.end());
// }


// bool	MessageValidator::isAuthenticationCommand(const std::string &command)
// {
// 	static const std::set<std::string>	authCommands = {"PASS", "NICK", "USER"};
	
// 	return (authCommands.find(command) != authCommands.end());
// }


// bool	MessageValidator::isChannelManagementCommand(const std::string &command)
// {
// 	static const std::set<std::string>	managementCommands = {"JOIN", "PART"}
	
// 	return (managementCommands.find(command) != managementCommands.end());
// }



// 

bool	MessageValidator::_isAlphanumeric(const std::string &str)
{
	size_t	i = 0;

	while (i < str.size())
	{
		if (!std::isalnum(str[i]))
			return (false);
		++i;
	}
	return (true);
}
