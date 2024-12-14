/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdemers <jdemers@student.42quebec.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 23:04:24 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/13 18:57:01 by jdemers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** add edge cases where a command should not return false even if not implemented ?
		-> handle CAP (ignore, do not throw error)..

*/

#include "Validator.hpp"
#include "parsing_utils.hpp"

#include <algorithm> // find
#include <cctype> // isalnum, isalpha
#include <iostream>
#include <stdexcept>

const size_t Validator::MAX_NICKNAME_LENGTH = 9;
const size_t Validator::MAX_CHANNEL_NAME_LENGTH = 42; // arbitrary limit for channels
const std::string	Validator::VALID_MODE_FLAGS = "+-itkol";

// ...other validations (for password rules, limits or allowed chars ?)

static std::map<std::string, CommandType>	initCommandMap(void)
{
	std::map<std::string, CommandType>	cmdMap;

	cmdMap["PASS"] = PASS;
	cmdMap["NICK"] = NICK;
	cmdMap["USER"] = USER;
	cmdMap["JOIN"] = JOIN;
	cmdMap["PART"] = PART;
	cmdMap["TOPIC"] = TOPIC;
	cmdMap["MODE"] = MODE;
	cmdMap["KICK"] = KICK;
	cmdMap["INVITE"] = INVITE;
	cmdMap["PRIVMSG"] = PRIVMSG;
	cmdMap["NOTICE"] = NOTICE;

	return (cmdMap);
}

const std::map<std::string, CommandType>	Validator::_commandMap = initCommandMap();

/* ************************************************************************** */

const Validator::ValidatorFunc	Validator::_validators[] = {
	&Validator::_validatePassCommand,   // PASS
	&Validator::_validateNickCommand,   // NICK
	&Validator::_validateUserCommand,   // USER
	&Validator::_validateJoinCommand,   // JOIN
	&Validator::_validatePartCommand,   // PART
	&Validator::_validateTopicCommand,  // TOPIC
	&Validator::_validateModeCommand,   // MODE
	&Validator::_validateKickCommand,   // KICK
	&Validator::_validateInviteCommand, // INVITE
	&Validator::_validatePrivmsgCommand,// PRIVMSG
	&Validator::_validateNoticeCommand  // NOTICE
};

/* ************************************************************************** */

// static std::map<ReplyType, std::string>	initErrorMessages(void)
// {
// 	std::map<ReplyType, std::string>	errMsgs;

// 	errMsgs[ERR_NONICKNAMEGIVEN] = "No nickname given";
// 	errMsgs[ERR_ERRONEUSNICKNAME] = "Erroneous nickname";
// 	// ...
	
// 	return (errMsgs);
// };

// const std::map<ReplyType, std::string>	Validator::_errorMessages = initErrorMessages();


/* ************************************************************************** */

Validator::Validator(void) : _error(static_cast<ReplyType>(0)), _errorArgs() {}
Validator::~Validator(void) {}

/* ************************************************************************** */

const std::map<std::string, CommandType>& Validator::getCommandMap(void)
{
	return (_commandMap);
}

ReplyType	Validator::getError(void) const
{
	return (_error);
}

const std::vector<std::string>&	Validator::getErrorArgs(void) const
{
	return (_errorArgs);
}

// CommandType	Validator::getCommandType(const std::string &cmd)
// {
// 	std::map<std::string, CommandType>::const_iterator	it = cmdMapPtr.find(cmd);
	
// 	if (it != cmdMapPtr.end())
// 		return (it->second);
// 	return (UNKNOWN);
// }

// std::string	Validator::getErrorReply(void) const
// {
// 	return (Reply().reply(_error.first, _error.second));
// } // missing some logic here...

/* ************************************************************************** */

/*	
Validates the syntax (structure and validity) of a command,
tokenized in a map of string key and values

Checks:
	"command" must exist and be a non-empty string,
	
	optional "prefix" must be a valid nickname
	validates the command syntax
	... ?

Errors:
	421 ERR_UNKNOWNCOMMAND: Unknown command
*/
// bool	Validator::validateCommand(const std::map<std::string, std::string> &command) const
// {
// 	// check if "command" key exists and has non-empty value
// 	if (command.find("command") == command.end() || command.at("command").empty())
// 	// if (_parsedMessage["command"].empty())
// 		return (_setError(ERR_UNKNOWNCOMMAND, "*"));
	
// 	const std::string	&cmd = command.at("command");

// 	// validate if the command exists in the map
// 	CommandType	cmdType = _getCommandType(cmd);
// 	if (cmdType == CMD_UNKNOWN)
// 		return (_setError(ERR_UNKNOWNCOMMAND, cmd));

// 	// validate optional prefix
// 	if (command.find("prefix") != command.end() && !command.at("prefix").empty())
// 	// if (!_parsedMessage["prefix"].empty())
// 	{
// 		if (!_isValidNickname(command.at("prefix")))
// 		// if (!_isValidNickname(_parsedMessage["prefix"])
// 		return (_setError(ERR_ERRONEUSNICKNAME, command.at("prefix"))); //?
// 	}
	
// 	// ... additional validation as needed
	
// 	return (validateCommandByType(cmdType, command));
// }


/* ************************************************************************** */

bool	Validator::validateCommand(const std::map<std::string, std::string> &command) const
{
	// check if "command" key exists and is non-empty
	if (command.find("command") == command.end() || command.at("command").empty())
	// if (_parsedMessage["command"].empty())
		return (_setError(ERR_UNKNOWNCOMMAND, "*"));
	
	const std::string	&cmd = command.at("command");

	// lookup the command type in the command map
	std::map<std::string, CommandType>::const_iterator	it = _commandMap.find(cmd);

	if (it == _commandMap.end())
		return (_setError(ERR_UNKNOWNCOMMAND, cmd));

	CommandType	cmdType = it->second;
	
	// validate optional prefix
	if (command.find("prefix") != command.end() && !command.at("prefix").empty())
	// if (!_parsedMessage["prefix"].empty())
	{
		if (!_isValidNickname(command.at("prefix")))
		// if (!_isValidNickname(_parsedMessage["prefix"])
		return (_setError(ERR_ERRONEUSNICKNAME, command.at("prefix"))); //?
	}
	
	// ... additional validation as needed
	
	return (_validateCommandByType(cmdType, command));
}

bool	Validator::_validateCommandByType(CommandType cmdType, const std::map<std::string, std::string> &command) const
{
	// if (cmdType == CMD_UNKNOWN && cmd == "CAP")
		// return (_noError()); // CAP command is ignored without error ??

	if (cmdType < PASS || cmdType > NOTICE)
		return (_setError(ERR_UNKNOWNCOMMAND, command.at("command")));

	return ((this->*(_validators[cmdType]))(command));
}

/*
Validates the command semantically based on its type
Uses command-specific methods (e.g., ValidateJoinCommand)
*/
// bool	Validator::_validateCommandByType(CommandType cmdType, const std::map<std::string, std::string> &command) const
// {
// 	switch (cmdType)
// 	{
// 		case PASS:
// 			return (_validatePassCommand(command));
// 		case NICK:
// 			return (_validateNickCommand(command));
// 		case USER:
// 			return (_validateUserCommand(command));
// 		case JOIN:
// 			return (_validateJoinCommand(command));
// 		case PART:
// 			return (_validatePartCommand(command));
// 		case TOPIC:
// 			return (_validateTopicCommand(command));
// 		case MODE:
// 			return (_validateModeCommand(command));
// 		case KICK:
// 			return (_validateKickCommand(command));
// 		case INVITE:
// 			return (_validateInviteCommand(command));
// 		case PRIVMSG:
// 			return (_validatePrivmsgCommand(command));
// 		case NOTICE:
// 			return (_validateNoticeCommand(command));
// 		default:
// 			return (_setError(ERR_UNKNOWNCOMMAND, command.at("command")));
// 	}
// }

/* ************************************************************************** */ // syntax validation

/*
Validate IRC nicknames

	Must start with a letter and contain only alphanumeric characters
	Must contain only alphanumeric characters, underscores, or dashes

*/
bool	Validator::_isValidNickname(const std::string &nickname) const
{
	if (nickname.empty() || nickname.length() > MAX_NICKNAME_LENGTH)
		return (false);

	// if (nickname.size() > 10)
		// return (false); // 432

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
bool	Validator::_isValidChannelName(const std::string &channel) const
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

	if (!_isValidNickname(command.at("params")))
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
	
	if (!_isValidChannelName(command.at("params")))
		return (_setError(ERR_NOSUCHCHANNEL, command.at("params")));

	// Additional checks like invite-only or channel limits can be implemented elsewhere
	return (_noError());
}

// bool Validator::_validateJoinCommand(const std::map<std::string, std::string> &command) const
// {
// 	if (command.find("params") == command.end() || command.at("params").empty())
// 		return(_setError(ERR_NEEDMOREPARAMS, "JOIN"));

// 	const std::string	&params = command.at("params");
// 	const std::string	trailing = "";
	
// 	if (command.find("trailing") != command.end())
// 		trailing = command.at("trailing");

// 	std::vector<std::pair<std::string, std::string> >	channelsAndKeys = parseChannelsAndKeys(params, trailing);
	
// 	size_t	i = 0;
// 	while (i < channelsAndKeys.size())
// 	{
// 		const std::string	&channel = channelsAndKeys[i].first;
// 		const std::string	&key = channelsAndKeys[i].second;

// 		if (!_isValidChannelName(channel))
// 			return (_setError(ERR_NOSUCHCHANNEL, channel));

// 		// channel existence and keys validation ?
		
// 		++i;
// 	}

// 	return (_noError());
// }

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
	
	if (!_isValidChannelName(command.at("params")))
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
	
	if (!_isValidChannelName(command.at("params")))
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
	std::string			invalidChars;

	// to return the first invalid char encountered
	size_t	i = 0;
	while (i < mode.size())
	{
		if (VALID_MODE_FLAGS.find(mode[i]) == std::string::npos) // if char is not in VALID_MODE_FLAGS
			return (_setError(ERR_UNKNOWNMODE, std::string(1, mode[i])));
		++i;
	}
	
	// to return only the invalid chars
	/*
	size_t	i = 0;
	while (i < mode.size())
	{
		if (VALID_MODE_FLAGS.find(mode[i]) == std::string::npos) // if char is not in VALID_MODE_FLAGS
			invalidChars += mode[i];
		++i;
	}
	if (!invalidChars.empty())
		return (_setError(ERR_UNKNOWNMODE, invalidChars));
	*/
	
	// to return the whole mode if one is invalid
	/*
	if (mode.find_first_not_of("+-itkol") != std::string::npos) // allowed mode chars
		return (_setError(ERR_UNKNOWNMODE, mode));
	*/

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

	if (!_isValidChannelName(params[0]))
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
	if (!_isValidNickname(recipient) && !_isValidChannelName(recipient))
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
		return (_setError(ERR_NORECIPIENT,command.at("prefix"), "NOTICE")); // second arg is to be the nickname..?

	if (command.find("trailing") == command.end() || command.at("trailing").empty())
		return (_setError(ERR_NOTEXTTOSEND, "NOTICE"));

	return (_noError());
}


/* ************************************************************************** */

/*
*/
// bool	Validator::_setError(int error, const std::string &arg1, const std::string &arg2) const
bool	Validator::_setError(ReplyType error, const std::string &arg1, const std::string &arg2) const
{
	_error = error;
	_errorArgs.clear();

	if (!arg1.empty())
		_errorArgs.push_back(arg1);
	if (!arg2.empty())
		_errorArgs.push_back(arg2);

	return (false);
}

/*
Resets the error state
*/
bool	Validator::_noError(void) const
{
	_error = static_cast<ReplyType>(0);
	_errorArgs.clear();

	return (true);
}

/* ************************************************************************** */ // ideas

// bool Validator::_validateNonEmpty(const std::string &valie, ReplyType error) const
// {
// 	if (value.empty())
// 	{
// 		return (_setError(error, "Value cannot be empty"));
// 	}
// 	return (true);
// }

// bool	Validator::_validateNickCommand(const std::map<std::string, std::string> &command) const
// {
// 	return (_validateNonEmpty(command["nickname"], ERR_NONICKNAMEGIVEN) \
// 	&& command["nickname"].size <= MAX_NICKNAME_LENGTH);
// }

/* ************************************************************************** */

/*
#include "Validator.hpp"
#include <map>
#include <string>
#include <iostream>

int main(void)
{
	Validator	validator;
	std::map<std::string, std::string>	command;

	// Test valid PASS command
	command["command"] = "PASS";
	command["argument"] = "password";
	if (validator.validateCommand(command))
		std::cout << "PASS command validated successfully!\n";
	else
		std::cout << "Error: " << validator.getError() << "\n";

	// Test invalid NICK command
	command["command"] = "NICK";
	command["argument"] = "verylongnickname";
	if (validator.validateCommand(command))
		std::cout << "NICK command validated successfully!\n";
	else
		std::cout << "Error: " << validator.getError() << "\n";

	return (0);
}
*/
