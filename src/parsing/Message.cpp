/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:57:01 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/12 09:10:04 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "parsing_utils.hpp"	// normalizeInput(), maybe trim(), tokenize()
#include "iomanip" // std::setw()

// Message::Message(void) {}
// Message::~Message(void) {}

// Message::Message(const Message &other)
// {
// 	*this = other;
// }

// Message&	Message::operator=(const Message &other)
// {
// 	if (this != &other)
// 	{
// 		// 
// 	}
// 	return (*this);
// }

/*

** Assumes the input is a single command, as it will remove any CRLF ("\r\n") characters from it
*/
Message::Message(const std::string &input) : _input(input), _reply(""), _parser(), _validator()
{
	_processInput(normalizeInput(input));
	// _processInput(trim(normalizeInput(input)));
}

/*
	// init keys with default values
	// tokenize the input
	// parse the tokens into the map
	// merge parsedCommand into _parsedMessage, keeping defaults for missing keys
	// validate the parsed command

*/
void	Message::_processInput(const std::string &input)
{
	_parsedMessage["prefix"] = "";
	_parsedMessage["command"] = "";
	_parsedMessage["params"] = "";
	_parsedMessage["trailing"] = "";

	std::vector<std::string>	tokens = tokenize(input);
	std::map<std::string, std::string>	parsedCommand = _parser.parseCommand(tokens);

	// change for the _parser.parse(input) instead *!? (handles tokenizing) **(but fix it first...)

	std::map<std::string, std::string>::iterator	it = parsedCommand.begin();
	while (it != parsedCommand.end())
	{
		_parsedMessage[it->first] = it->second;
		++it;
	}

	Reply	rpl; // should we put it as private in Validator class ? or inside the try/if's body to not instanciate rpl if not needed
	try
	{
		if (!_validator.validateCommand(_parsedMessage))
		{
			ReplyType	errorCode = _validator.getError();
			const std::vector<std::string>	&args = _validator.getErrorArgs();
			_reply = rpl.reply(errorCode, args);
			return ; // to prevent reply format related errors from overriding _reply
		}
	}
	catch (const std::exception &e)
	{
		_reply = rpl.reply(ERR_UNKNOWNCOMMAND, "PROCESSING", "ERROR", e.what());
	}

	// // (wip) if the command is "JOIN", parse channels and keys // other commands also ?
	// if (_parsedMessage["command"] == "JOIN")
	// {
	// 	std::string	params = _parsedMessage["params"];
	// 	std::string	trailing = _parsedMessage["trailing"];
	// 	_channelsAndKeys = parseChannelsAndKeys(params, trailing);
	// }
	
	_reply.clear(); // if everything is valid
}

const std::string	&Message::getInput(void) const { return (_input); }
const std::string	&Message::getPrefix(void) const { return (_parsedMessage.at("prefix")); }
const std::string	&Message::getCommand(void) const { return (_parsedMessage.at("command")); }
const std::string	&Message::getParams(void) const { return (_parsedMessage.at("params")); }
const std::string	&Message::getTrailing(void) const { return (_parsedMessage.at("trailing")); }
const std::string	&Message::getReply(void) const { return (_reply); }

// const std::vector<std::pair<std::string, std::string> >	&Message::getChannelsAndKeys(void) const
// {
// 	return (_channelsAndKeys);
// }

/*	TOCHECK: do we implement the channelsAndKeys conditional display?

Displays the Message object's attributes,
** Used for debug
*/
std::ostream	&operator<<(std::ostream &oss, const Message &message)
{
	const int	labelWidth = 18;
	
	oss << "Message Details:\n" <<
		GRAYTALIC << std::setw(labelWidth) << "Input:  " << RESET << message.getInput() << "\n"
		GRAYTALIC << std::setw(labelWidth) << "Prefix:  " << RESET << message.getPrefix() << "\n"
		GRAYTALIC << std::setw(labelWidth) << "Command:  " << RESET << message.getCommand() << "\n"
		GRAYTALIC << std::setw(labelWidth) << "Params:  " << RESET << message.getParams() << "\n"
		GRAYTALIC << std::setw(labelWidth) << "Trailing:  " << RESET << message.getTrailing() << "\n"
		GRAYTALIC << std::setw(labelWidth) << "Reply:  " << RESET << message.getReply();
	
	return (oss);
}
