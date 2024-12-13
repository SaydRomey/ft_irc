/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:57:01 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/13 04:06:13 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "parsing_utils.hpp"	// normalizeInput(), maybe trim(), tokenize()
#include <iomanip>				// std::setw()

Message::Message(void) : _valid(false), _input(""), _reply("") {}
Message::~Message(void) {}

Message::Message(const Message &other)
	: _valid(false),
	  _input(other._input),
	  _parsedMessage(other._parsedMessage),
	  _reply(other._reply),
	  _channelsAndKeys(other._channelsAndKeys)
{
	// might need to implement assignable for Parser and Validator .. ?
	// _parser(other._parser),
	// _validator(other._validator)
	// *this = other;
}

/*	** Assumes the input is a single command (will remove any CRLF ("\r\n") characters from it)
*/
Message::Message(const std::string &input) : _valid(false), _input(input), _reply("")
{
	_processInput(normalizeInput(input));
	// _processInput(trim(normalizeInput(input)));
}

Message&	Message::operator=(const Message &other)
{
	if (this != &other)
	{
		_valid = other._valid;
		_input = other._input;
		_parsedMessage = other._parsedMessage;
		_reply = other._reply;
		_channelsAndKeys = other._channelsAndKeys;
		// _parser = other._parser;
		// _validator = other._validator;
	}
	return (*this);
}

/* ************************************************************************** */

const std::string	&Message::getInput(void) const { return (_input); }
const std::string	&Message::getPrefix(void) const { return (_parsedMessage.at("prefix")); }
const std::string	&Message::getCommand(void) const { return (_parsedMessage.at("command")); }
const std::string	&Message::getParams(void) const { return (_parsedMessage.at("params")); }
const std::string	&Message::getTrailing(void) const { return (_parsedMessage.at("trailing")); }
const std::string	&Message::getReply(void) const { return (_reply); }

const std::vector<std::pair<std::string, std::string> >	&Message::getChannelsAndKeys(void) const
{
	return (_channelsAndKeys);
}

bool	Message::isValid(void) const { return (_valid); }

/* ************************************************************************** */

/*
Focuses on the high-level flow:
parsing, validating and delegating to command-specific handlers
*/
void	Message::_processInput(const std::string &input)
{
	_parsedMessage = _parser.parseCommand(input);

	if (!_validateParsedCommand())
		return ;
	
	if (_parsedMessage["command"] == "JOIN")
		_processJoinCommand();

	_valid = true;	
	_reply.clear(); // if everything is valid
}

/*
Handles command validation and error reporting
*/
bool	Message::_validateParsedCommand(void)
{
	try
	{
		if (!_validator.validateCommand(_parsedMessage))
		{
			Reply	rpl;
			_reply = rpl.reply(_validator.getRplType(), _validator.getRplArgs());
			_valid = false;
			return (false);
		}
	}
	catch (const std::exception &e)
	{
		Reply	rpl;
		_reply = rpl.reply(ERR_UNKNOWNCOMMAND, "PROCESSING", "ERROR", e.what());
		_valid = false;
		return (false);
	}
	return (true);
}

void	Message::_processJoinCommand(void)
{
	if (!hasMultipleEntries(_parsedMessage["params"]))
		return ;

	try
	{
		// std::vector<std::pair<std::string, std::string> >	channelsAndKeys;
		
		// channelsAndKeys = _parser.parseChannelsAndKeys(_parsedMessage["params"]);
		// _channelsAndKeys = channelsAndKeys;
		
		_channelsAndKeys = _parser.parseChannelsAndKeys(_parsedMessage["params"]);
	}
	catch (const std::exception &e)
	{
		Reply	rpl;
		_reply = rpl.reply(ERR_UNKNOWNCOMMAND, "JOIN", "Invalid params", e.what());
		_valid = false;
	}
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
		printLabeledField(oss, "Params: ", params, labelWidth);
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
				oss << GRAYTALIC << std::setw(labelWidth) << "  Key (" << (j + 1) << "): " << RESET << pairs[j].second << "\n";
			}
			else
			{
				oss << GRAYTALIC << std::setw(labelWidth) << "  Key (" << (j + 1) << "): " << RESET << "<none>" << "\n";
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
