/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:57:01 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/20 15:00:38 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "parsing_utils.hpp"	// normalizeInput(), maybe trim(), tokenize()
#include <iomanip>				// std::setw()

Message::Message(void) : _valid(false), _nickname("*"), _input(""), _reply("") {}

// Message::Message(const std::string &input)
// 	: _valid(false), _nickname("*"), _input(input), _reply("")
// {
// 	_processInput(normalizeInput(input));
// 	// _processInput(trim(normalizeInput(input)));	
// }

Message::Message(const std::string &input, const std::string &nickname)
	: _valid(false), _nickname(nickname), _input(input), _reply("")
{
	// if (!isValidNickname(nickname))
	// 	_nickname = "*";
	
	// std::cout << YELLOW << "*** Input before normalize and trim:\t" << RESET << ">" << input << "<\n";
	// std::cout << YELLOW << "*** Input after normalize:\t\t" << RESET << ">" << normalizeInput(input) << "<\n";
	// std::cout << YELLOW << "*** Input after trim:\t\t\t" << RESET << ">" << trim(input) << "<\n";
	// std::cout << YELLOW << "*** Input after normalize and trim:\t" << RESET << ">" << trim(normalizeInput(input)) << "<\n";

	// _processInput(normalizeInput(input));
	_processInput(trim(normalizeInput(input)));
}

Message::Message(const Message &other)
	: _valid(other._valid),
	  _nickname(other._nickname),
	  _input(other._input),
	  _parsedMessage(other._parsedMessage),
	  _reply(other._reply),
	  _channelsAndKeys(other._channelsAndKeys),
	  _tokenizedParams(other._tokenizedParams)
{
	// might need to implement assignable for Parser and Validator .. ?
	// _parser(other._parser),
	// _validator(other._validator)
	// *this = other;
}

Message&	Message::operator=(const Message &other)
{
	if (this != &other)
	{
		_valid = other._valid;
		_nickname = other._nickname;
		_input = other._input;
		_parsedMessage = other._parsedMessage;
		_reply = other._reply;
		_channelsAndKeys = other._channelsAndKeys;
		_tokenizedParams = other._tokenizedParams;
		// _parser = other._parser;
		// _validator = other._validator;
	}
	return (*this);
}

Message::~Message(void) {}

/* ************************************************************************** */

bool	Message::isValid(void) const { return (_valid); }

const std::string	&Message::getInput(void) const { return (_input); }

const std::string	&Message::getPrefix(void) const
{
	// if (_parsedMessage.at("prefix") == "*")
		// return ("");
	return (_parsedMessage.at("prefix"));
}

const std::string	&Message::getCommand(void) const { return (_parsedMessage.at("command")); }
const std::string	&Message::getParams(void) const { return (_parsedMessage.at("params")); }
const t_vecStr		&Message::getParamsVec(void) const { return (_tokenizedParams); }
// const std::string	&Message::getParams(bool tokenized) const { return (_tokenizedParams); }
const std::string	&Message::getTrailing(void) const { return (_parsedMessage.at("trailing")); }
const std::string	&Message::getReply(void) const { return (_reply); }

const std::vector<std::pair<std::string, std::string> >	&Message::getChannelsAndKeys(void) const
{
	return (_channelsAndKeys);
}

/* ************************************************************************** */

/*
Focuses on the high-level flow:
parsing, validating and delegating to command-specific handlers
*/
void	Message::_processInput(const std::string &input)
{
	// Debug: trace raw input
	std::cout << YELLOW << "**DEBUG: Processing input: " << RESET << input << std::endl;
	
	// // guard against empty input
	// if (input.empty())
	// {
	// 	_reply = reply(ERR_UNKNOWNCOMMAND, "*", "*Empty input*");
	// 	_valid = false;
	// 	return ;
	// }

	try
	{
		_parsedMessage = _parser.parseCommand(input);

		// Debug: print parsed message structure
		std::cout << YELLOW << "**DEBUG: Parsed message structure:" << RESET << std::endl;
		std::map<std::string, std::string>::iterator	it = _parsedMessage.begin();
		while (it != _parsedMessage.end())
		{
			std::cout << YELLOW << "  Key: " << it->first << ", Value: " << (it->second.empty() ? "<empty>" : it->second) << std::endl;
			++it;
		}

		// assign default prefix if not provided
		std::map<std::string, std::string>::iterator	prefixIt = _parsedMessage.find("prefix");
		if(prefixIt == _parsedMessage.end() || prefixIt->second.empty())
			_parsedMessage["prefix"] = _nickname;
		
		// validate the parsed command
		if (!_validator.validateCommand(_parsedMessage))
		{
			_reply = reply(_validator.getRplType(), _validator.getRplArgs());
			_valid = false;
			return ;
		}

		// extract command and params
		std::map<std::string, std::string>::iterator	commandIt = _parsedMessage.find("command");
		if (commandIt == _parsedMessage.end())
		{
			_reply = reply(ERR_UNKNOWNCOMMAND, "*", "*Missing command*");
			_valid = false;
			return ;
		}
		const std::string	&command = commandIt->second;

		std::map<std::string, std::string>::iterator	paramsIt = _parsedMessage.find("params");
		const std::string	params = (paramsIt != _parsedMessage.end()) ? paramsIt->second : "";

		// dispatch to command-specific handlers
		if (command == "JOIN" && countTokens(paramsIt->second) > 1)
			_processJoinCommand(params);
		else if (command == "KICK")
			_processKickCommand(params);
		else
		{
			std::cout << YELLOW \
			<< "**DEBUG: (currently) No specific command handler for: " \
			<< RESET << command << std::endl;
			
			_tokenizedParams = tokenize(params);
		}

		_valid = true;
		_reply.clear();
	}
	catch (const std::exception &e)
	{
		std::string	reason = e.what();
		if (reason.empty())
			reason = "EMPTY REASON";
		
		_reply = reply(ERR_UNKNOWNERROR, "*", "[command]", reason);
		_valid = false;
	}
	
	
	// _parsedMessage = _parser.parseCommand(input);

	// if(_parsedMessage.at("prefix").empty())
	// 	_parsedMessage["prefix"] = _nickname;
	
	// if (!_validator.validateCommand(_parsedMessage))
	// {
	// 	_reply = reply(_validator.getRplType(), _validator.getRplArgs());
	// 	_valid = false;
	// 	return ;
	// }

	// const std::string	&command = _parsedMessage.at("command");
	// const std::string	&params = _parsedMessage.at("params");
		
	// if (command == "JOIN")
	// 	_processJoinCommand(params);
	// // else if (command == "PART")
	// // 	_processPartCommand(params);
	// else if (command == "KICK")
	// 	_processKickCommand(params);
	// // else if (command == "MODE")
	// // 	_processModeCommand(params);
	// // else if (command == "TOPIC")
	// // 	_processTopicCommand(params);
	// // else if (command == "INVITE")
	// // 	_processInviteCommand(params);
	// // else if (command == "PRIVMSG" || command == "NOTICE")
	// // 	_processPrivMsgCommand(params);
	// else
	// 	_tokenizedParams = tokenize(params);

	// // 	PART params are either 1 or more channels..
	// // 	INVITE invitee_nickname #channel
	// // 	PRIVMSG and NOTICE 1 or more targets
	
	// // Tokenize general params for other commands

	// _valid = true;	
	// _reply.clear(); // if everything is valid
}

/*
*/
void	Message::_processJoinCommand(const std::string &params)
{
	// Debug: trace raw params
	std::cout << YELLOW << "**Debug: Processing JOIN params: " << RESET << params << std::endl;
	
	if (hasValidNumberOfParams(params, 2, AT_MOST))
	{
		_channelsAndKeys = _parser.parseChannelsAndKeys(params);

		// Debug: print parsed channels and keys
		size_t	i = 0;
		while (i < _channelsAndKeys.size())
		{
			std::cout << YELLOW \
			<< "Channel " << (i + 1) << ": " << _channelsAndKeys[i].first \
			<< ", key: " << (_channelsAndKeys[i].second.empty() ? "<none>" : _channelsAndKeys[i].second) \
			<< RESET << std::endl;
			++i;
		}
	}
	// else
	// {
	// 	_reply = reply(ERR_NEEDMOREPARAMS, "*", "JOIN");
	// 	_valid = false;
	// }
}

void	Message::_processKickCommand(const std::string &params)
{
	// Debug: trace raw params
	std::cout << YELLOW << "**Debug: Processing KICK params: " << RESET << params << std::endl;

	if (hasValidNumberOfParams(params, 2, EXACTLY))
	{
		t_vecStr	paramsTokens = tokenize(params);
		std::string	channel = paramsTokens[0];
		
		t_vecStr	kickParams;
		kickParams.push_back(channel);
		
		if (hasMultipleEntries(paramsTokens[1]))
		{
			t_vecStr	userTokens = tokenize(paramsTokens[1], ',', true);
			kickParams.insert(kickParams.end(), userTokens.begin(), userTokens.end());
		}
		else
		{
			kickParams.push_back(paramsTokens[1]);
		}

		// Debug: trace parsed KICK parameters
		size_t	i = 0;
		while (i < kickParams.size())
		{
			std::cout << YELLOW \
			<< "Kick param " << (i + 1) << ": " \
			<< kickParams[i] << RESET << std::endl;
			++i;
		}

		_tokenizedParams = kickParams;
	}
	// else
	// {
	// 	_reply = reply(ERR_NEEDMOREPARAMS, "*", "KICK");
	// 	_valid = false;
	// }
}

/* ************************************************************************** */

/*
Displays the Message object's attributes,
** Used for debug
*/
// std::ostream	&operator<<(std::ostream &oss, const Message &message)
// {
// 	const int	labelWidth = 18;
	
// 	oss << "Message Details:\n" <<
// 		GRAYTALIC << std::setw(labelWidth) << "Input:  " << RESET << message.getInput() << "\n"
// 		GRAYTALIC << std::setw(labelWidth) << "Prefix:  " << RESET << message.getPrefix() << "\n"
// 		GRAYTALIC << std::setw(labelWidth) << "Command:  " << RESET << message.getCommand() << "\n"
// 		GRAYTALIC << std::setw(labelWidth) << "Params:  " << RESET << message.getParams() << "\n"
// 		GRAYTALIC << std::setw(labelWidth) << "Trailing:  " << RESET << message.getTrailing() << "\n"
// 		GRAYTALIC << std::setw(labelWidth) << "Reply:  " << RESET << message.getReply();
	
// 	return (oss);
// }

/*	** Used for debug

Displays the Message object's attributes
Conditionnal alternative display for multi params
Conditionnal alternative display for multi params with "JOIN" as channel/key pairs

*/
// std::ostream &operator<<(std::ostream &oss, const Message &message)
// {
// 	const int labelWidth = 18;

// 	// Print basic details
// 	oss << "Message Details:\n"
// 		<< GRAYTALIC << std::setw(labelWidth) << "Input: " << RESET << message.getInput() << "\n"
// 		<< GRAYTALIC << std::setw(labelWidth) << "Prefix: " << RESET << message.getPrefix() << "\n"
// 		<< GRAYTALIC << std::setw(labelWidth) << "Command: " << RESET << message.getCommand() << "\n";

// 	// Tokenize params by spaces
// 	std::vector<std::string> paramsTokens = tokenize(message.getParams(), ' ');

// 	// Print params based on the number of tokens
// 	if (paramsTokens.size() > 1)
// 	{
// 		size_t i = 0;
// 		while (i < paramsTokens.size())
// 		{
// 			oss << GRAYTALIC << std::setw(labelWidth - 4) << "Param (" << (i + 1) << "): " << RESET << paramsTokens[i] << "\n";
// 			++i;
// 		}
// 	}
// 	else
// 	{
// 		oss << GRAYTALIC << std::setw(labelWidth) << "Params: " << RESET << message.getParams() << "\n";
// 	}

// 	oss << GRAYTALIC << std::setw(labelWidth) << "Trailing: " << RESET << message.getTrailing() << "\n"
// 		<< GRAYTALIC << std::setw(labelWidth) << "Reply: " << RESET << message.getReply() << "\n";

// 	// Conditionally print the channel/key pairs if the command is "JOIN" and the params are multi
// 	if (message.getCommand() == "JOIN" && hasMultipleEntries(message.getParams()))
// 	{
// 		oss << "\nChannels & Keys:\n";

// 		const std::vector<std::pair<std::string, std::string> > &pairs = message.getChannelsAndKeys();

// 		size_t	j = 0;
// 		while (j < pairs.size())
// 		{
// 			oss << GRAYTALIC << std::setw(labelWidth) << "  Channel (" << (j + 1) << "): " << RESET << pairs[j].first << "\n";

// 			if (!pairs[j].second.empty())
// 			{
// 				oss << GRAYTALIC << std::setw(labelWidth) << "  Key (" << (j + 1) << "): " << RESET << pairs[j].second << "\n";
// 			}
// 			else
// 			{
// 				oss << GRAYTALIC << std::setw(labelWidth) << "  Key (" << (j + 1) << "): " << RESET << "<none>" << "\n";
// 			}
// 			++j;
// 		}
// 	}

// 	return (oss);
// }

/* ************************************************************************** */

// Helper function to print labeled fields
static void	printLabeledField(std::ostream &oss, const std::string &label, const std::string &value, int labelWidth)
{
	oss << GRAYTALIC << std::setw(labelWidth) << label << RESET << value << "\n";
}

// Handle multi-parameter output
static void	handleMultiParams(std::ostream &oss, const std::string &params, int labelWidth)
{
	std::vector<std::string>	paramsTokens = tokenize(params, ' ');

	if (paramsTokens.size() > 1)
	{
		size_t	i = 0;
		while (i < paramsTokens.size())
		{
			oss << GRAYTALIC << std::setw(labelWidth) << "Param (" << (i + 1) << "): " << RESET << paramsTokens[i] << "\n";
			++i;
		}
	}
	else
	{
		printLabeledField(oss, "Params: ", params, labelWidth + 4);
	}
}

// Handle channels and keys output
static void	handleChannelsAndKeys(std::ostream &oss, const Message &message, int labelWidth)
{
	if (message.getCommand() == "JOIN" && hasMultipleEntries(message.getParams()))
	{
		oss << "\nChannels & Keys:\n";

		const std::vector<std::pair<std::string, std::string> >	&pairs = message.getChannelsAndKeys();

		size_t	j = 0;
		while (j < pairs.size())
		{
			oss << GRAYTALIC << std::setw(labelWidth) << "  Channel (" << (j + 1) << "): " << RESET << pairs[j].first << "\n";

			if (!pairs[j].second.empty())
			{
				oss << GRAYTALIC << std::setw(labelWidth) << "  Key (" << (j + 1) << "): " << RESET << pairs[j].second << "\n\n";
			}
			else
			{
				oss << GRAYTALIC << std::setw(labelWidth) << "  Key (" << (j + 1) << "): " << RESET << "<none>" << "\n\n";
			}
			++j;
		}
	}
}

std::ostream	&operator<<(std::ostream &oss, const Message &message)
{
	const int	labelWidth = 18;

	// Print basic details
	oss << "Message Details:\n";
	printLabeledField(oss, "Valid: ", message.isValid() ? "True" : "False", labelWidth);
	printLabeledField(oss, "Input: ", message.getInput(), labelWidth);
	printLabeledField(oss, "Prefix: ", message.getPrefix(), labelWidth);
	printLabeledField(oss, "Command: ", message.getCommand(), labelWidth);

	// Handle multi-parameter output
	handleMultiParams(oss, message.getParams(), labelWidth - 4);

	// Print trailing and reply
	printLabeledField(oss, "Trailing: ", message.getTrailing(), labelWidth);
	printLabeledField(oss, "Reply: ", message.getReply(), labelWidth);

	// Handle channels and keys
	handleChannelsAndKeys(oss, message, labelWidth);

	return (oss);
}
