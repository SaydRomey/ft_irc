
/*
** add edge cases where a command should not return false even if not implemented ?
		-> handle CAP (ignore, do not throw error)..

*/

#include "Validator.hpp"
#include "parsing_utils.hpp" // ?

#include <algorithm>	// find
#include <cctype>		// isalnum, isalpha
#include <iostream>
#include <stdexcept>

const size_t 		Validator::MAX_NICKNAME_LENGTH = 9;
const size_t 		Validator::MAX_CHANNEL_NAME_LENGTH = 42;
const std::string	Validator::VALID_MODE_FLAGS = "+-itkol";
const std::string	Validator::INVALID_CHANNEL_CHARS = " ,\r\n";

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
	&Validator::_validatePassCommand,
	&Validator::_validateNickCommand,
	&Validator::_validateUserCommand,
	&Validator::_validateJoinCommand,
	&Validator::_validatePartCommand,
	&Validator::_validateTopicCommand,
	&Validator::_validateModeCommand,
	&Validator::_validateKickCommand,
	&Validator::_validateInviteCommand,
	&Validator::_validatePrivmsgCommand,
	&Validator::_validateNoticeCommand
};

/* ************************************************************************** */

Validator::Validator(void) : _rplType(static_cast<ReplyType>(0)), _rplArgs() {}
Validator::Validator(const Validator &other) : _rplType(other._rplType), _rplArgs(other._rplArgs) {}
Validator&	Validator::operator=(const Validator &other)
{
	if (this != &other)
	{
		_rplType = other._rplType;
		_rplArgs = other._rplArgs;
	}
	return (*this);
}
Validator::~Validator(void) {}

/* ************************************************************************** */

ReplyType	Validator::getRplType(void) const
{
	return (_rplType);
}

const std::vector<std::string>&	Validator::getRplArgs(void) const
{
	return (_rplArgs);
}

const std::map<std::string, CommandType>& Validator::getCommandMap(void)
{
	return (_commandMap);
}

/* ************************************************************************** */

bool	Validator::_setRpl(ReplyType rplType, const std::string &arg1, const std::string &arg2, const std::string &arg3, const std::string &arg4) const
{
	_rplType = rplType;
	_rplArgs.clear();

	_rplArgs = makeArgs(arg1, arg2, arg3, arg4);
	
	return (false);
}

/*
Resets the error state
*/
bool	Validator::_noRpl(void) const
{
	_rplType = static_cast<ReplyType>(0);
	_rplArgs.clear();

	return (true);
}

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
bool	Validator::validateCommand(const std::map<std::string, std::string> &command) const
{
	// check if "command" key exists and is non-empty
	if (command.find("command") == command.end() || command.at("command").empty())
	// if (_parsedMessage["command"].empty())
		return (_setRpl(ERR_UNKNOWNCOMMAND, command.at("prefix"), "*"));
	
	const std::string	&cmd = command.at("command");

	// lookup the command type in the command map
	std::map<std::string, CommandType>::const_iterator	it = _commandMap.find(cmd);

	if (it == _commandMap.end())
		return (_setRpl(ERR_UNKNOWNCOMMAND, command.at("prefix"), cmd));

	CommandType	cmdType = it->second;
	
	// // validate optional prefix
	// if (command.find("prefix") != command.end() && !command.at("prefix").empty())
	// // if (!_parsedMessage["prefix"].empty())
	// {
	// 	if (!_isValidNickname(command.at("prefix")))
	// 	// if (!_isValidNickname(_parsedMessage["prefix"])
	// 	return (_setRpl(ERR_ERRONEUSNICKNAME, command.at("prefix"), )); //? is this check needed ? we might not input prefixes at all as users..
	// }
	
	// ... additional validation as needed
	
	return (_validateCommandByType(cmdType, command));
	// return ((this->*(_validators[cmdType]))(command)); // to remove _validateCommandByType() ?
}

bool	Validator::_validateCommandByType(CommandType cmdType, const std::map<std::string, std::string> &command) const
{
	// if (cmdType == CMD_UNKNOWN && cmd == "CAP")
		// return (_noRpl()); // CAP command is ignored without error ??

	if (cmdType < PASS || cmdType > NOTICE) // will not get here -> in validateCommand(), we iterate through _commandMap...
		return (_setRpl(ERR_UNKNOWNCOMMAND, command.at("command")));

	return ((this->*(_validators[cmdType]))(command));
}

/* ************************************************************************** */ // syntax validation

/*	** https://dd.ircdocs.horse/refs/commands/nick

Validate IRC nickname

	Must start with a letter,
	Must contain only alphanumeric characters, underscores, or dashes,
	Must not exceed specified limit
*/
bool	Validator::_isValidNickname(const std::string &nickname) const
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

bool	isValidNickname(const std::string &nickname)
{
	if (nickname.empty() || nickname.length() > 9)
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
bool	Validator::_isValidChannelName(const std::string &channel) const
{
	if (channel.empty() || channel.length() > MAX_CHANNEL_NAME_LENGTH)
		return (false);

	if (channel[0] != '#')
		return (false);

	if (channel.find_first_of(INVALID_CHANNEL_CHARS) != std::string::npos)
		return (false);

	return (true);
}

// bool	Validator::isValidPassword(const std::string &password) const
// {
// 	// 
// 	return (_setRpl(ERR_PASSWDMISMATCH));
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

Success Reply:
No specific reply is defined in IRC for a successful PASS command,
but it’s implied as part of the connection process.
Ensure errors like ERR_NEEDMOREPARAMS or ERR_ALREADYREGISTERED are handled.
*/
bool	Validator::_validatePassCommand(const std::map<std::string, std::string> &command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (_setRpl(ERR_NEEDMOREPARAMS, "PASS"));

	return (_noRpl());
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

Success Reply:
	None specified,
	but errors like ERR_ALREADYREGISTERED should be handled.
*/
bool	Validator::_validateNickCommand(const std::map<std::string, std::string> &command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (_setRpl(ERR_NONICKNAMEGIVEN));

	if (!_isValidNickname(command.at("params")))
		return (_setRpl(ERR_ERRONEUSNICKNAME, command.at("prefix"), command.at("params")));

	// Check if nickname is already in use (higher-level logic) 462
	// return (_setRpl(RPL_WELCOME, command.at("params"))); // TOCHANGE
	return (_noRpl());
}

/*
Validate "USER" command

	"params" must exist and have at least 4 tokens: username, hostname, servername, realname

Errors:
	461 ERR_NEEDMOREPARAMS: Not enough parameters
	462 ERR_ALREADYREGISTERED: You may not reregister

Success Reply:
	None specified for successful NICK changes,
	but server behavior should update the nickname state.
*/
bool Validator::_validateUserCommand(const std::map<std::string, std::string> &command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return(_setRpl(ERR_NEEDMOREPARAMS, "USER"));

	// tokenize parameters
	std::vector<std::string> params = tokenize(command.at("params"));

	// append trailing if it exists
	if (command.find("trailing") != command.end())
		params.push_back(command.at("trailing"));
	
	if (params.size() < 4)
		return(_setRpl(ERR_NEEDMOREPARAMS, command.at("prefix"), command.at("command")));

	// Check if the client is already registered (higher-level logic)
	return (_noRpl());
}

/*
Validate "JOIN" command

	"params" must exist and contain a valid channel name

Errors:
	403 ERR_NOSUCHCHANNEL: No such channel
	405 ERR_TOOMANYCHANNELS: You have joined too many channels
	407 ERR_TOOMANYTARGETS: Duplicate recipients. No message delivered
	461 ERR_NEEDMOREPARAMS: Not enough parameters
	471 ERR_CHANNELISFULL: Cannot join channel (+l)
	473 ERR_INVITEONLYCHAN: Cannot join channel (+i)
	475 ERR_BADCHANNELKEY: Cannot join channel (+k)
	476 ERR_BADCHANMASK: Bad channel mask

Success Reply:
	join message? (RPL_JOIN, custom reply..)
	RPL_TOPIC (332)	or	RPL_NOTOPIC (331)
	RPL_NAMEREPLY (353)
	RPL_ENDOFNAMES (366)
*/
bool Validator::_validateJoinCommand(const std::map<std::string, std::string> &command) const
{
	std::map<std::string, std::string>::const_iterator	prefixIt = command.find("prefix");
	std::string	prefix = (prefixIt != command.end() && !prefixIt->second.empty()) ? prefixIt->second : "*";
	
	std::map<std::string, std::string>::const_iterator	paramsIt = command.find("params");
	
	if (paramsIt == command.end() || paramsIt->second.empty())
		return (_setRpl(ERR_NEEDMOREPARAMS, prefix, "JOIN"));

	// should we ignore trailing if present, or flag the error?

	// extract and tokenize "params"
	std::string	params = paramsIt->second;
	std::vector<std::string>	paramsTokens = tokenize(params, ' ');
	
	// validate number of tokens in "params"
	if (paramsTokens.size() > 2)
		return (_setRpl(ERR_UNKNOWNCOMMAND, prefix, "JOIN"));

	// validate channels (first parameter)
	std::vector<std::string>	channelTokens = tokenize(paramsTokens[0], ',');

	size_t	i = 0;
	while (i < channelTokens.size())
	{
		if (!_isValidChannelName(channelTokens[i]))
			return (_setRpl(ERR_NOSUCHCHANNEL, prefix, channelTokens[i]));
		++i;
	}

	return (_noRpl());
}

/*
Validate "PART" command

	"params" must exist and contain a valid channel name

Errors:
	403 ERR_NOSUCHCHANNEL: No such channel
	442 ERR_NOTONCHANNEL: You're not on that channel
	461 ERR_NEEDMOREPARAMS: Not enough parameters

Success Reply:
	Typically no numeric reply;
	success is implied by a PART message broadcasted to the channel.
*/
bool Validator::_validatePartCommand(const std::map<std::string, std::string>& command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (_setRpl(ERR_NEEDMOREPARAMS, command.at("prefix"), "PART"));

	std::string	params = command.at("params");

	std::vector<std::string>	channelTokens = tokenize(params, ','); // limit?

	size_t	i = 0;
	while (i < channelTokens.size())
	{
		if (!_isValidChannelName(channelTokens[i]))
			return (_setRpl(ERR_NOSUCHCHANNEL, command.at("prefix"), channelTokens[i]));
		++i;
	}

	// Check if the user is on the channel (handled in higher-level logic)
	return (_noRpl());
}

/*
Validate "TOPIC" command

	"params" must exist and contain a valid channel name

Errors:
	403 ERR_NOSUCHCHANNEL: You're not on that channel
	461 ERR_NEEDMOREPARAMS: Not enough parameters
	482 ERR_CHANOPRIVSNEEDED: You're not channel operator

Success Reply:
	RPL_TOPIC (332)
		Sent when the topic is successfully set or retrieved.
	
	RPL_NOTOPIC (331)
		Sent if there is no topic.	
*/
bool Validator::_validateTopicCommand(const std::map<std::string, std::string>& command) const
{
	std::map<std::string, std::string>::const_iterator	paramsIt = command.find("params");
	std::map<std::string, std::string>::const_iterator	prefixIt = command.find("prefix");

	const std::string	&prefix = (prefixIt != command.end()) ? prefixIt->second : "*";

	// check if "params" is missing or empty
	if (paramsIt == command.end() || paramsIt->second.empty())
		return (_setRpl(ERR_NEEDMOREPARAMS, prefix, "TOPIC"));
	
	// check if the "params" value is not a valid channel name
	if (!_isValidChannelName(paramsIt->second))
		return (_setRpl(ERR_NOSUCHCHANNEL, prefix, paramsIt->second));

	// Check if the user is on the channel (higher-level logic)
	return (_noRpl());
}

/*
Checks the validity of mode parameters, considering:

+k: Requires a key.
+o: Requires a nickname.
+l: Requires a numeric limit.
*/
bool	Validator::_isValidModeParam(char modeFlag, const std::string &param) const
{
	switch (modeFlag)
	{
		case 'k': // key mode requires a non-empty key
			return (!param.empty());

		case 'o': // operator mode requires a valid nickname
			return (Validator()._isValidNickname(param));

		case 'l': // limit mode requires a positive numeric value
		{
			if (param.find_first_not_of("0123456789") != std::string::npos)
				return (false);
			
			return (!param.empty() && std::atoi(param.c_str()) > 0);
		}
	
		// modes without parameters (i, t) do not need additional validation
		case 'i':
		case 't':
			return (true);

		default:
			return (false); // unknown mode flag.. (already checked)
	}
}

/*	** https://modern.ircdocs.horse//#channel-modes

Validate "MODE" command

Errors:
	461 ERR_NEEDMOREPARAMS: Not enough parameters
	472 ERR_UNKNOWNMODE: is unknown mode char to me
	696 ERR_INVALIDMODEPARAM: Invalid parameter for mode

(checked in a higher level function):
	441 ERR_USERNOTINCHANNEL: They aren't on that channel
	467 ERR_KEYSET: Channel key already set
	482 ERR_CHANOPRIVSNEEDED: You're not channel operator

Success Reply:
	RPL_CHANNELMODEIS (324): // info for this reply in higher level function
		Sent to show the current mode of the channel.
	Broadcast mode changes to the channel as appropriate.
*/
bool Validator::_validateModeCommand(const std::map<std::string, std::string>& command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (_setRpl(ERR_NEEDMOREPARAMS, command.at("prefix"), command.at("command")));

	std::vector<std::string>	paramsTokens = tokenize(command.at("params"));

	// ensure channel is specified
	if (paramsTokens.size() < 2)
		return (_setRpl(ERR_NEEDMOREPARAMS, command.at("prefix"), "MODE"));

	const std::string	&channel = paramsTokens[0];
	const std::string	&modes = paramsTokens[1];
	size_t	paramIndex = 2; // start checking for additional params after 'modes'

	// validate target (channel)
	if (!_isValidChannelName(channel))
		return (_setRpl(ERR_BADCHANMASK, command.at("prefix"), channel));
	
	// bool	addMode = true;
	size_t	i = 0;
	while (i < modes.size())
	{
		char	modeFlag = modes[i];
		
		// toggle add/remove mode
		// if (modeFlag == '+')
		// 	addMode = true;
		// else if (modeFlag == '-')
		// 	addMode = false;
		// else
		// {
			if (VALID_MODE_FLAGS.find(modeFlag) == std::string::npos)
				return (_setRpl(ERR_UNKNOWNMODE, command.at("prefix"), std::string(1, modeFlag)));
			
			// check if the mode requires a parameter
			if (modeFlag == 'k' || modeFlag == 'o' || modeFlag == 'l')
			{
				// make sure a parameter exists for the mode
				if (paramIndex >= paramsTokens.size())
					return (_setRpl(ERR_NEEDMOREPARAMS, command.at("prefix"), "MODE"));

				const std::string &param = paramsTokens[paramIndex];
				if (!_isValidModeParam(modeFlag, param))
					// return (_setRpl(ERR_INVALIDMODEPARAM, makeArgs(channel, std::string(1, modeFlag), param)));
					return (_setRpl(ERR_INVALIDMODEPARAM, command.at("prefix"), channel, std::string(1, modeFlag), param));
				++paramIndex;
			}
		// }
		++i;
	}
	
	// Check for the 3 param limit for modes with parameters
	if (paramIndex - 2 > 3)
		return (_setRpl(ERR_UNKNOWNERROR, command.at("prefix"), "Too many parameterized modes"));
	
	return (_noRpl());

	// to return only the invalid chars
	/*
	std::string	invalidChars;
	size_t	i = 0;
	while (i < modes.size())
	{
		if (VALID_MODE_FLAGS.find(modes[i]) == std::string::npos) // if char is not in VALID_MODE_FLAGS
			invalidChars += modes[i];
		++i;
	}
	if (!invalidChars.empty())
		return (_setRpl(ERR_UNKNOWNMODE, invalidChars));
	*/
}

/*
Validate "KICK" command

	"params" must exist and contain the target and channel

Errors:
	403 ERR_NOSUCHCHANNEL: No such channel
	441 ERR_USERNOTINCHANNEL: They aren't on that channel
	442 ERR_NOTONCHANNEL: You're not on that channel
	461 ERR_NEEDMOREPARAMS: Not enough parameters
	476 ERR_BADCHANMASK: Bad channel mask
	482 ERR_CHANOPRIVSNEEDED: You're not channel operator

Success Reply:
	No specific numeric reply;
	success is implied by broadcasting the KICK message to the channel.
*/
bool Validator::_validateKickCommand(const std::map<std::string, std::string> &command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (_setRpl(ERR_NEEDMOREPARAMS, command.at("prefix"), "KICK"));

	std::vector<std::string> paramsTokens = tokenize(command.at("params"));
	
	if (paramsTokens.size() < 2)
		return (_setRpl(ERR_NEEDMOREPARAMS, command.at("prefix"), "KICK"));

	std::string	channels = paramsTokens[0];
	std::string	users = paramsTokens[1];

	std::vector<std::string>	channelTokens = tokenize(channels, ',');
	size_t	i = 0;
	while (i < channelTokens.size())
	{
		if (!_isValidChannelName(channelTokens[i]))
			return (_setRpl(ERR_NOSUCHCHANNEL, command.at("prefix"), channelTokens[i]));
		++i;
	}
	
	std::vector<std::string>	userTokens = tokenize(users, ',');
	size_t	j = 0;
	while (j < userTokens.size())
	{
		if (!_isValidNickname(userTokens[j]))
			return (_setRpl(ERR_NONICKNAMEGIVEN, command.at("prefix"), userTokens[j]));
		++j;
	}

	// Check if the target user is in the channel (higher-level logic)
	return (_noRpl());
}

/*
invite
params must exist and specify the target user and channel

Errors:
	401 ERR_NOSUCHNICK: No such nick/channel
	403 ERR_NOSUCHCHANNEL: No such channel ?
	442 ERR_NOTONCHANNEL: You're not on that channel
	443 ERR_USERONCHANNEL: is already on channel
	461 ERR_NEEDMOREPARAMS: Not enough parameters
	482 ERR_CHANOPRIVSNEEDED: You're not channel operator

Success Reply:
	RPL_INVITING (341):
		Sent to acknowledge a successful invite.

Optionally notify the invitee with a server message.
*/
bool Validator::_validateInviteCommand(const std::map<std::string, std::string>& command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (_setRpl(ERR_NEEDMOREPARAMS, command.at("prefix"), "INVITE"));

	std::vector<std::string>	paramsTokens = tokenize(command.at("params"));

	if (paramsTokens.size() < 2)
		return (_setRpl(ERR_NEEDMOREPARAMS, command.at("prefix"), "INVITE"));
	

	// Validate that the user and channel exist (additional logic needed)
	return (_noRpl());
}

/*	** TODO: fix wrong _setRpl arguments for 411 and 412...

Validate "PRIVMSG" command

	"params" must exist and contain the recipient
	"trailing" must exist and contain the message body

Errors:
	401 ERR_NOSUCHNICK: No such nick/channel
	404 ERR_CANNOTSENDTOCHAN: Cannot send to channel
	407 ERR_TOOMANYTARGETS: Duplicate recipients. No message delivered
	411 ERR_NORECIPIENT: No recipient given (PRIVMSG)
	412 ERR_NOTEXTTOSEND: No text to send

Success Reply:
	No numeric reply;
	success is implied by delivering the message.
*/
bool Validator::_validatePrivmsgCommand(const std::map<std::string, std::string> &command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (_setRpl(ERR_NORECIPIENT, command.at("prefix"), command.at("command")));

	if (command.find("trailing") == command.end() || command.at("trailing").empty())
		return (_setRpl(ERR_NOTEXTTOSEND, command.at("prefix")));

	std::string	params = command.at("params");
	std::vector<std::string>	recipients = tokenize(params, ',');

	size_t	i = 0;
	while (i < recipients.size())
	{
		const std::string	&recipient = recipients[i];
		if (!_isValidNickname(recipient) && !_isValidChannelName(recipient))
			return (_setRpl(ERR_NOSUCHNICK, command.at("prefix"), recipient));
		++i;
	}
		
	// Additional checks for recipient existence can be handled elsewhere
	// same for channel existence..
	
	return (_noRpl());
}

/*
similar to privmsg but does not return (errors to the sender

Success Reply:
	No numeric reply;
		success is implied by delivering the notice.
		Avoid acknowledgment per IRC protocol rules.
*/
bool Validator::_validateNoticeCommand(const std::map<std::string, std::string>& command) const
{
	if (command.find("params") == command.end() || command.at("params").empty())
		return (_setRpl(ERR_NORECIPIENT,command.at("prefix"), "NOTICE"));

	if (command.find("trailing") == command.end() || command.at("trailing").empty())
		return (_setRpl(ERR_NOTEXTTOSEND, command.at("prefix"), "NOTICE"));

	std::string	params = command.at("params");
	std::vector<std::string>	recipients = tokenize(params, ',');

	size_t	i = 0;
	while (i < recipients.size())
	{
		const std::string	&recipient = recipients[i];
		if (!_isValidNickname(recipient) && !_isValidChannelName(recipient))
		{
			return (_setRpl(ERR_NOSUCHNICK, command.at("prefix"), recipient));
		}
		++i;
	}

	return (_noRpl()); // todo: find a higher level way of ignoring silently instead if cmd is invalid..
}

/* ************************************************************************** */ // ideas

// bool Validator::_validateNonEmpty(const std::string &valie, ReplyType error) const
// {
// 	if (value.empty())
// 	{
// 		return (_setRpl(error, "Value cannot be empty"));
// 	}
// 	return (true);
// }

// bool	Validator::_validateNickCommand(const std::map<std::string, std::string> &command) const
// {
// 	return (_validateNonEmpty(command["nickname"], ERR_NONICKNAMEGIVEN) && command["nickname"].size <= MAX_NICKNAME_LENGTH);
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

/* ************************************************************************** */ // **TOCHECK where do we put this?

/*	** TOCHECK: do we prevent a username, nickname to be the same as a command ?

// Check if nickname is already in use (higher-level logic) 462

*/
bool	isNickAvailable(const std::map<std::string, int> &nickMap, const std::string &nickToCheck)
{
	std::string	nickname = nickToCheck;
	
	std::string::iterator	nickIt = nickname.begin();
	while (nickIt != nickname.end())
	{
		*nickIt = static_cast<char>(std::tolower(*nickIt));
		++nickIt;
	}

	std::map<std::string, int>::const_iterator	it = nickMap.begin();
	while (it != nickMap.end())
	{
		std::string	nickKey = it->first;
		std::string::iterator	keyIt = nickKey.begin();
		while (keyIt != nickKey.end())
		{
			*keyIt = static_cast<char>(std::tolower(*keyIt));
			++keyIt;
		}
		if (nickKey == nickname)
		{
			return (false);
		}
		++it;
	}
	return (true);
}
