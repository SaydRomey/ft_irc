/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageValidator.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:48:44 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/30 02:31:24 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageValidator.hpp"
#include "_parsing_utils.hpp"
#include <cctype>
#include <set>

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

MessageValidator::MessageValidator(void) {}

/*
Focus only on validation for commands, nicknames, channels, and trailing
*/
bool	MessageValidator::isValidCommand(const std::map<std::string, std::string> &command)
{
	static const std::string	commandsArray[] = {
		"PASS", "NICK", "USER", "JOIN", "PART", "TOPIC", "MODE",
		"KICK", "INVITE", "PRIVMSG", "NOTICE"
	};

	static const std::set<std::string>	validCommands(commandsArray, commandsArray + sizeof(commandsArray) / sizeof(commandsArray[0]));
	
	if (!command.count("command") || command.at("command").empty())
	{
		return (false);
	}
	
	const std::string	&cmd = command.at("command");

	return (validCommands.count(cmd) > 0);
}

/* TODO: add nickname syntax rules
*/
bool	MessageValidator::isValidNickname(const std::string &nickname)
{
	if (nickname.empty() || !std::isalpha(nickname[0]))
	{
		return (false); // must start with a letter
	}
	
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

bool	MessageValidator::isValidTrailing(const std::string &trailing)
{
	return (!trailing.empty()); // basic check  for now..
}

/* *? TODO:
*/
// bool	MessageValidator::isValidModeCommand(const std::vector<std::string> &params)
// {
// 	// must start with '+' or '-' and trail one char at least (mode char validity will be handled by Channel class)
// 	if (params.size() < 2)
// 		return (false);
	
// 	const std::string &channel = params[0];
	
// 	return (isValidChannel(channel));
// }

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
