/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 23:04:24 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/02 00:39:58 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Validator.hpp"
#include <cctype> // isalnum, isalpha
#include <stdexcept>
#include <algorithm> // find

const size_t Validator::MAX_NICKNAME_LENGTH = 9;
const size_t Validator::MAX_CHANNEL_NAME_LENGTH = 42; // arbitrary limit for channels

static std::set<std::string> _initValidCommands(void)
{
	std::set<std::string>	commands;
	
	commands.insert("PASS");
	commands.insert("NICK");
	commands.insert("USER");
	commands.insert("JOIN");
	commands.insert("PART");
	commands.insert("TOPIC");
	commands.insert("MODE");
	commands.insert("KICK");
	commands.insert("INVITE");
	commands.insert("PRIVMSG");
	commands.insert("NOTICE");
	
	return (commands);
}

const std::set<std::string>	Validator::_validCommands = _initValidCommands();

Validator::Validator(void) {}
Validator::~Validator(void) {}

/*
Validate the structure and validity of a command
*/
bool	Validator::isValidCommand(const std::map<std::string, std::string> &command) const
{
	// check if "command" exists and is non-empty
	if (command.find("command") == command.end() || command.at("command").empty())
		return (false);

	// check optional prefix validity
	if (command.find("prefix") != command.end() && !isValidNickname(command.at("prefix")))
		return (false);

	// check if the command is one of the valid commands
	const std::string	&cmd = command.at("command");
	
	if (_validCommands.find(cmd) == _validCommands.end())
		return (false);
	
	// ... additional validation as needed
	
	return (true);
}

/*
Validate the command semantically based on its type
Use command-specific methods (e.g., ValidateJoinCommand) to verify proper usage
*/
bool	Validator::validateCommand(const std::map<std::string, std::string> &command) const
{
	const std::string	&cmd = command.at("command");

	if (cmd == "JOIN")
		return (_validateJoinCommand(command));
	else if (cmd == "PRIVMSG")
		return (_validatePrivmsgCommand(command));

	// ... validation for other commands as needed
	// else if (cmd == "KICK")
		// return (_validateKickCommand(command));
	
	return (false); // unknown commands are considered invalid
}

/*
Validate IRC nicknames

- Must start with a letter and contain only alphanumeric characters
- Must contain only alphanumeric characters, underscores, or dashes

*/
bool	Validator::isValidNickname(const std::string &nickname) const
{
	if (nickname.empty() || nickname.length() > MAX_NICKNAME_LENGTH)
		return (false);

	if (!std::isalpha(nickname[0]))
		return (false);
	
	size_t	i = 1;
	while (i < nickname.length())
	{
		if (!std::isalnum(nickname[i]) && nickname[i] != '_' && nickname[i] != '-')
			return (false);
		++i;
	}
	
	return (true);
}

/*
Validate IRC channel names

- Must start with '#'
- Must not contain invalid characters:
	(' ', ','. '\r', '\n') 

*/
bool	Validator::isValidChannelName(const std::string& channel) const
{
	if (channel.empty() || channel.length() > MAX_CHANNEL_NAME_LENGTH)
		return (false);

	if (channel[0] != '#')
		return (false);

	size_t	i = 1;
	while (i < channel.length())
	{
		if (channel[i] == ' ' || channel[i] == ',' || channel[i] == '\r' || channel[i] == '\n')
			return (false);
		++i;
	}
	
	return (true);
}

/*
Validate "JOIN" command

- "params" must exist and contain a valid channel name

*/
bool	Validator::_validateJoinCommand(const std::map<std::string, std::string> &command) const
{
	if (command.find("params") == command.end() || !isValidChannelName(command.at("params")))
		return (false);
	
	return (true);
}

/*
Validate "PRIVMSG" command

- "params" must exist and contain the recipient
- "trailing" must exist and contain the message body

*/
bool Validator::_validatePrivmsgCommand(const std::map<std::string, std::string> &command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (false);
	
	if (command.find("trailing") == command.end() || command.at("trailing").empty())
		return (false);
	
	return (true);
}
