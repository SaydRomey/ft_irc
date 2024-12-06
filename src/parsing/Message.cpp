/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:57:01 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/06 11:58:34 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "parsing_utils.hpp"

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

Message::Message(const std::string &input) : _input(input), _reply(), _parser(), _validator()
{
	_processInput(normalizeInput(input));
	// _processInput(trim(normalizeInput(input)));
}

void	Message::_processInput(const std::string &input)
{
	// init keys with default values
	_parsedMessage["prefix"] = "";
	_parsedMessage["command"] = "";
	_parsedMessage["params"] = "";
	_parsedMessage["trailing"] = "";

	// tokenize the input
	std::vector<std::string>	tokens = tokenize(input);
	
	// parse the tokens into the map
	std::map<std::string, std::string>	parsedCommand = _parser.parseCommand(tokens);

	// merge parsedCommand into _parsedMessage, keeping defaults for missing keys
	std::map<std::string, std::string>::iterator	it = parsedCommand.begin();
	while (it != parsedCommand.end())
	{
		_parsedMessage[it->first] = it->second;
		++it;
	}

	// validate the parsed command
	Reply	rpl;
	try
	{
		if (input.empty())
		{
			_reply = rpl.reply(ERR_UNKNOWNCOMMAND, "*");
			return ;
		}
		if (!_validator.isValidCommand(_parsedMessage)) // syntax validation
		{
			_reply = rpl.reply(ERR_UNKNOWNCOMMAND, _parsedMessage["command"]);
			return ;
		}
		if (!_validator.validateCommand(_parsedMessage)) // semantic validation
		{
			ReplyType	errorCode = _validator.getError();
			const std::vector<std::string>	&args = _validator.getErrorArgs();
			_reply = rpl.reply(errorCode, args);
			return ;
		}
	}
	catch (const std::exception &e)
	{
		_reply = rpl.reply(ERR_UNKNOWNCOMMAND, "PROCESSING", "ERROR", e.what());
	}
	
	// if everything is valid
	_reply.clear();
}

const std::string	&Message::getInput(void) const {	return (_input); }
const std::string	&Message::getPrefix(void) const { return (_parsedMessage.at("prefix")); }
const std::string	&Message::getCommand(void) const { return (_parsedMessage.at("command")); }
const std::string	&Message::getParams(void) const { return (_parsedMessage.at("params")); }
const std::string	&Message::getTrailing(void) const { return (_parsedMessage.at("trailing")); }
const std::string	&Message::getReply(void) const {	return (_reply); }

std::ostream	&operator<<(std::ostream &oss, const Message &message)
{
	oss << "Message Details:\n"
		<< "  Input:\t\"" << message.getInput() << "\"\n"
		<< "  Prefix:\t" << message.getPrefix() << "\n"
		<< "  Command:\t" << message.getCommand() << "\n"
		<< "  Params:\t" << message.getParams() << "\n"
		<< "  Trailing:\t" << message.getTrailing() << "\n"
		<< "  Reply:\t" << message.getReply();
	
	return (oss);
}
