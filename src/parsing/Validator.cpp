/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 23:04:24 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/06 12:35:58 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Validator.hpp"
#include "parsing_utils.hpp"

#include <algorithm> // find
#include <cctype> // isalnum, isalpha
#include <iostream>
#include <stdexcept>

const size_t Validator::MAX_NICKNAME_LENGTH = 9;
const size_t Validator::MAX_CHANNEL_NAME_LENGTH = 42; // arbitrary limit for channels
// ...other validations (for password or allowed chars ?)

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

Validator::Validator(void) : _error(RPL_VALID), _errorArgs() {}
Validator::~Validator(void) {}

/*
Validate the structure and validity of a command

	Checks:
	"command" must exist and be valid
	optional "prefix" must be a valid nickname
	validates the command syntax

Errors:
	421 ERR_UNKNOWNCOMMAND: Unknown command
*/
bool	Validator::isValidCommand(const std::map<std::string, std::string> &command) const
{
	// check if "command" exists and is non-empty
	if (command.find("command") == command.end() || command.at("command").empty())
		return (false);

	const std::string	&cmd = command.at("command");
	
	if (_validCommands.find(cmd) == _validCommands.end())
		return (false);
	
	if (command.find("prefix") != command.end() && !command.at("prefix").empty())
	{
		if (!isValidNickname(command.at("prefix")))
		return (false); // invalid prefix (no specific error code..)
	}
	
	// ... additional validation as needed
	
	return (true);
}

/*	** add edge cases where a command should not return false even if not implemented ?

Validate the command semantically based on its type
Use command-specific methods (e.g., ValidateJoinCommand) to verify proper usage
*/
bool	Validator::validateCommand(const std::map<std::string, std::string> &command) const
{
	const std::string	&cmd = command.at("command");
	
	if (cmd == "PASS")
		return (_validatePassCommand(command));
	else if (cmd == "NICK")
		return (_validateNickCommand(command));
	else if (cmd == "USER")
		return (_validateUserCommand(command));
	else if (cmd == "JOIN")
		return (_validateJoinCommand(command));
	else if (cmd == "PART")
		return (_validatePartCommand(command));
	else if (cmd == "TOPIC")
		return (_validateTopicCommand(command));
	else if (cmd == "MODE")
		return (_validateModeCommand(command));
	else if (cmd == "KICK")
		return (_validateKickCommand(command));
	else if (cmd == "INVITE")
		return (_validateInviteCommand(command));
	else if (cmd == "PRIVMSG")
		return (_validatePrivmsgCommand(command));
	else if (cmd == "NOTICE")
		return (_validateNoticeCommand(command));
	
	// handle CAP (ignore, do not throw error)..
	// ... ?
	
	return (_setError(ERR_UNKNOWNCOMMAND, command.at("command")));
}

/* ************************************************************************** */ // syntax validation

/*
Validate IRC nicknames

	Must start with a letter and contain only alphanumeric characters
	Must contain only alphanumeric characters, underscores, or dashes

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

	Must start with '#'
	Must not contain invalid characters:
	(' ', ','. '\r', '\n') 

*/
bool	Validator::isValidChannelName(const std::string &channel) const
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

// bool	Validator::isValidPassword(const std::string &password) const
// {
// 	// 
// 	return (_setError(ERR_PASSWDMISMATCH));
// }

/* ************************************************************************** */

ReplyType	Validator::getError(void) const
{
	return (_error);
}

const std::vector<std::string>&	Validator::getErrorArgs(void) const
{
	return (_errorArgs);
}

/* ************************************************************************** */

/*
Validate "PASS" command

	"params" must exist and contain a non-empty password

Errors:
	461 ERR_NEEDMOREPARAMS: Not enough parameters
	462 ERR_ALREADYREGISTERED: You may not reregister

** should we implement a password strength validator or rules?
ex:
[Please try again with a more obscure password. 
Passwords should be at least five characters long,
should not be something easily guessed
(e.g. your real name or your nick),
and cannot contain the space or tab characters]
**

*/
bool	Validator::_validatePassCommand(const std::map<std::string, std::string> &command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (_setError(ERR_NEEDMOREPARAMS, "PASS"));

	// Check if the client has already registered (handled in higher-level logic) 462
	return (_noError());
}

/*
Nicknames are non-empty strings with the following restrictions:

They MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A), question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40).
They MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A).
They MUST NOT start with a character listed as a channel type, channel membership prefix, or prefix listed in the IRCv3 multi-prefix Extension.
They SHOULD NOT contain any dot character ('.', 0x2E).
Servers MAY have additional implementation-specific nickname restrictions and SHOULD avoid the use of nicknames which are ambiguous with commands or command parameters where this could lead to confusion or error.
*/
/*
Validate "NICK" command

	"params" must exist and contain a valid nickname

Errors:
	431 ERR_NONICKNAMEGIVEN: No nickname given
	432 ERR_ERRONEUSNICKNAME: Erroneous nickname
	433 ERR_NICKNAMEINUSE: Nickname is already in use
*/
bool	Validator::_validateNickCommand(const std::map<std::string, std::string> &command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (_setError(ERR_NONICKNAMEGIVEN));

	if (!isValidNickname(command.at("params")))
		return (_setError(ERR_ERRONEUSNICKNAME, command.at("params")));

	// Check if nickname is already in use (higher-level logic) 462
	return (_noError());
}

/*
Validate "USER" command

	"params" must exist and have at least 4 tokens: username, hostname, servername, realname

Errors:
	461 ERR_NEEDMOREPARAMS: Not enough parameters
	462 ERR_ALREADYREGISTERED: You may not reregister

*/
bool Validator::_validateUserCommand(const std::map<std::string, std::string> &command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return(_setError(ERR_NEEDMOREPARAMS, "USER"));

	// tokenize parameters
	std::vector<std::string> params = tokenize(command.at("params"));

	// append trailing if it exists
	if (command.find("trailing") != command.end())
		params.push_back(command.at("trailing"));
	
	if (params.size() < 4)
		return(_setError(ERR_NEEDMOREPARAMS, command.at("command")));

	// Check if the client is already registered (higher-level logic)
	return (_noError());
}

/*
Validate "JOIN" command

	"params" must exist and contain a valid channel name

Errors:
	461 ERR_NEEDMOREPARAMS: Not enough parameters
	403 ERR_NOSUCHCHANNEL: No such channel
*/
bool Validator::_validateJoinCommand(const std::map<std::string, std::string> &command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return(_setError(ERR_NEEDMOREPARAMS, "JOIN"));
	
	if (!isValidChannelName(command.at("params")))
		return (_setError(ERR_NOSUCHCHANNEL, command.at("params")));

	// Additional checks like invite-only or channel limits can be implemented elsewhere
	return (_noError());
}

/*
Validate "PART" command

	"params" must exist and contain a valid channel name

Errors:
	461 ERR_NEEDMOREPARAMS: Not enough parameters
	403 ERR_NOSUCHCHANNEL: No such channel
	471 ERR_CHANNELISFULL: Cannot join channel (+l)
	473 ERR_INVITEONLYCHAN: Cannot join channel (+i)
*/
bool Validator::_validatePartCommand(const std::map<std::string, std::string>& command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (_setError(ERR_NEEDMOREPARAMS, "PART"));
	
	if (!isValidChannelName(command.at("params")))
		return (_setError(ERR_NOSUCHCHANNEL, command.at("params")));

	// Check if the user is on the channel (handled in higher-level logic)
	return (_noError());
}

/*
Validate "TOPIC" command

	"params" must exist and contain a valid channel name

Errors:
	461 ERR_NEEDMOREPARAMS: Not enough parameters
	403 ERR_NOSUCHCHANNEL: No such channel ?
	...?
	
*/
bool Validator::_validateTopicCommand(const std::map<std::string, std::string>& command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (_setError(ERR_NEEDMOREPARAMS, "TOPIC"));
	
	if (!isValidChannelName(command.at("params")))
		return (_setError(ERR_NOSUCHCHANNEL, command.at("params")));

	// Check if the user is on the channel (higher-level logic)
	return (_noError());
}

/*
mode
params must exist and specify a channel or user?
*/
bool Validator::_validateModeCommand(const std::map<std::string, std::string>& command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (_setError(ERR_NEEDMOREPARAMS, command.at("command")));

	std::vector<std::string>	params = tokenize(command.at("params"));

	// ensure channel or user is specified
	if (params.size() < 2)
		return (_setError(ERR_NEEDMOREPARAMS, "MODE"));

	const std::string	&mode = params[1];
	if (mode.find_first_not_of("+-itkol") != std::string::npos) // allowed mode chars
		return (_setError(ERR_UNKNOWNMODE, mode));

	// Validate mode syntax and permissions (additional logic needed)
	return (_noError());
}

/*
Validate "KICK" command

	"params" must exist and contain the target and channel

Errors:
	403 ERR_NOSUCHCHANNEL: No such channel
	441 ERR_USERNOTINCHANNEL: User not in channel
	461 ERR_NEEDMOREPARAMS: Not enough parameters
*/
bool Validator::_validateKickCommand(const std::map<std::string, std::string> &command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (_setError(ERR_NEEDMOREPARAMS, "KICK"));

	std::vector<std::string> params = tokenize(command.at("params"));
	if (params.size() < 2)
		return (_setError(ERR_NEEDMOREPARAMS, "KICK"));

	if (!isValidChannelName(params[0]))
		return (_setError(ERR_NOSUCHCHANNEL, command.at("params")));

	// Check if the target user is in the channel (higher-level logic)
	return (_noError());
}

/*
invite
params must exist and specify the target user and channel
*/
bool Validator::_validateInviteCommand(const std::map<std::string, std::string>& command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (_setError(ERR_NEEDMOREPARAMS, "INVITE"));

	std::vector<std::string>	params = tokenize(command.at("params"));

	if (params.size() < 2)
		return (_setError(ERR_NEEDMOREPARAMS, "INVITE"));

	// Validate that the user and channel exist (additional logic needed)
	return (_noError());
}

/*
Validate "PRIVMSG" command

	"params" must exist and contain the recipient
	"trailing" must exist and contain the message body

Errors:
	401 ERR_NOSUCHNICK: No such nick/channel
	411 ERR_NORECIPIENT: No recipient given (PRIVMSG)
	412 ERR_NOTEXTTOSEND: No text to send
*/
bool Validator::_validatePrivmsgCommand(const std::map<std::string, std::string> &command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (_setError(ERR_NORECIPIENT, command.at("prefix"), command.at("command"))); // second arg is to be the nickname..?

	if (command.find("trailing") == command.end() || command.at("trailing").empty())
		return (_setError(ERR_NOTEXTTOSEND, command.at("command")));
	
	const std::string	&recipient = command.at("params");
	if (!isValidNickname(recipient) && !isValidChannelName(recipient))
		return (_setError(ERR_NOSUCHNICK, recipient));
	
	// Additional checks for recipient existence can be handled elsewhere
	// same for channel existence..
	
	return (_noError());
}

/*
similar to privmsg but does not return (errors to the sender
*/
bool Validator::_validateNoticeCommand(const std::map<std::string, std::string>& command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (_setError(ERR_NORECIPIENT, "NOTICE"));

	if (command.find("trailing") == command.end() || command.at("trailing").empty())
		return (_setError(ERR_NOTEXTTOSEND, "NOTICE"));

	return (_noError());
}

/* ************************************************************************** */

bool	Validator::_setError(ReplyType error, const std::string &arg1, const std::string &arg2) const
{
	_error = error;
	_errorArgs.clear();

	if (!arg1.empty())
		_errorArgs.push_back(arg1);
	if (!arg2.empty())
		_errorArgs.push_back(arg2);

	// std::cout << "_setError: " << error << " Args: " << arg1 << ", " << arg2 << std::endl;

	return (false);
}

bool	Validator::_noError(void) const
{
	_error = RPL_VALID;
	_errorArgs.clear();

	return (true);
}
