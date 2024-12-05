/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 23:04:24 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/05 13:47:21 by cdumais          ###   ########.fr       */
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

Validator::Validator(void) : _error(ERR_UNKNOWNCOMMAND) {}
Validator::~Validator(void) {}

/*
Validate the structure and validity of a command

- Checks:
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
		return (false); // 421

	const std::string	&cmd = command.at("command");
	
	if (_validCommands.find(cmd) == _validCommands.end())
		return (false); // 421
	
	if (command.find("prefix") != command.end() && !isValidNickname(command.at("prefix")))
		return (false); // invalid prefix (no specific error code..)
	
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
	
	return (false); // unknown commands (add edge cases where a command should not return false even if not implemented ?)
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
Validate "PASS" command

	"params" must exist and contain a non-empty password

Errors:
	461 ERR_NEEDMOREPARAMS: Not enough parameters
	462 ERR_ALREADYREGISTERED: You may not reregister
*/
bool	Validator::_validatePassCommand(const std::map<std::string, std::string> &command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (false); // 461

	// Check if the client has already registered (handled in higher-level logic)
	return (true);
}

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
		return (false); // 431

	if (!isValidNickname(command.at("params")))
		return (false); // 432

	// Check if nickname is already in use (higher-level logic)
	return (true);
}

/*
Validate "USER" command

- "params" must exist and have at least 4 tokens: username, hostname, servername, realname

Errors:
- 461 ERR_NEEDMOREPARAMS: Not enough parameters
- 462 ERR_ALREADYREGISTERED: You may not reregister
*/
bool Validator::_validateUserCommand(const std::map<std::string, std::string> &command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (false); // 461 ERR_NEEDMOREPARAMS

	std::vector<std::string> params = _tokenize(command.at("params")));
	if (params.size() < 4)
		return (false); // 461 ERR_NEEDMOREPARAMS

	// Check if the client is already registered (higher-level logic)
	return (true);
}

/*
Validate "JOIN" command

- "params" must exist and contain a valid channel name

Errors:
- 403 ERR_NOSUCHCHANNEL: No such channel
- 471 ERR_CHANNELISFULL: Cannot join channel (+l)
- 473 ERR_INVITEONLYCHAN: Cannot join channel (+i)
*/
bool Validator::_validateJoinCommand(const std::map<std::string, std::string> &command) const
{
	if (command.find("params") == command.end() || !isValidChannelName(command.at("params")))
		return (_setError(ERR_NOSUCHCHANNEL, command.at("params")));

	_error = RPL_WELCOME;
	_errorArgs.clear();
	
	// Additional checks like invite-only or channel limits can be implemented elsewhere
	return (true);
}

/*
Validate "PRIVMSG" command

- "params" must exist and contain the recipient
- "trailing" must exist and contain the message body

Errors:
- 401 ERR_NOSUCHNICK: No such nick/channel
- 411 ERR_NORECIPIENT: No recipient given (PRIVMSG)
- 412 ERR_NOTEXTTOSEND: No text to send
*/
bool Validator::_validatePrivmsgCommand(const std::map<std::string, std::string> &command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (_setError(ERR_NORECIPIENT, "PRIVMSG"));

	if (command.find("trailing") == command.end() || command.at("trailing").empty())
		return (_setError(ERR_NOTEXTTOSEND, "PRIVMSG"));
	
	_error = RPL_WELCOME; // *!! implement a default or reset error ?
	_errorArgs.clear();

	// Additional checks for recipient existence can be handled elsewhere
	return (true);
}

/*
Validate "KICK" command

- "params" must exist and contain the target and channel

Errors:
- 403 ERR_NOSUCHCHANNEL: No such channel
- 441 ERR_USERNOTINCHANNEL: User not in channel
- 461 ERR_NEEDMOREPARAMS: Not enough parameters
*/
bool Validator::_validateKickCommand(const std::map<std::string, std::string> &command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (false); // 461 ERR_NEEDMOREPARAMS

	std::vector<std::string> params = _tokenize(command.at("params")));
	if (params.size() < 2)
		return (false); // 461 ERR_NEEDMOREPARAMS

	if (!isValidChannelName(params[0]))
		return (false); // 403 ERR_NOSUCHCHANNEL

	// Check if the target user is in the channel (higher-level logic)
	return (true);
}

/*
part
params must exist and contain a valid channel name
*/
bool Validator::_validatePartCommand(const std::map<std::string, std::string>& command) const
{
	if (command.find("params") == command.end() || !isValidChannelName(command.at("params")))
		return (false); // 403 ERR_NOSUCHCHANNEL

	// Check if the user is on the channel (handled in higher-level logic)
	return (true);
}

/*
topic
params must exist and specify a channel
*/
bool Validator::_validateTopicCommand(const std::map<std::string, std::string>& command) const
{
	if (command.find("params") == command.end() || !isValidChannelName(command.at("params")))
		return (false); // 403 ERR_NOSUCHCHANNEL

	// Check if the user is on the channel (higher-level logic)
	return (true);
}

/*
mode
params must exist and specify a channel or user?
*/
bool Validator::_validateModeCommand(const std::map<std::string, std::string>& command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (false); // 403 ERR_NOSUCHCHANNEL

	// Validate mode syntax and permissions (additional logic needed)
	return (true);
}

/*
invite
params must exist and specify the target user and channel
*/
bool Validator::_validateInviteCommand(const std::map<std::string, std::string>& command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (false); // 403 ERR_NOSUCHCHANNEL

	// Validate that the user and channel exist (additional logic needed)
	return (true);
}

/*
similar to privmsg but does not return (errors to the sender
*/
bool Validator::_validateNoticeCommand(const std::map<std::string, std::string>& command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (false); // 411 ERR_NORECIPIENT

	if (command.find("trailing") == command.end() || command.at("trailing").empty())
		return (false); // 412 ERR_NOTEXTTOSEND

	return (true);
}

bool	Validator::_setError(ReplyType error, const std::string &rg1, const std::string &arg2)
{
	_error = error;
	_errorArgs.clear();

	if (!arg1.empty())
		_errorArgs.push_back(arg1);
	if (!arg2.empty())
		_errorArgs.push_back(arg2);

	return (false);
}
