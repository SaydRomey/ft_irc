/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:11:39 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/05 23:31:54 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Reply.hpp"

#include <stdexcept>

Parser::Parser(void) {}
Parser::~Parser(void) {}

/*
Single entry point (Parser::parse) to handle:

Tokenizing:	split the input
Parsing:	extract and organize command data
Validating:	validate command-specific details

Returns a Message object for valid inputs.
Throws a formatted reply for invalid inputs.
*/
// Message	Parser::parse(const std::string &input)
// {
// 	std::vector<std::string>	tokens = tokenize(input);
// 	std::map<std::string, std::string>	command = _parseCommand(tokens);
	
// 	Validator	validator;
// 	Reply		rpl;

// 	if (!validator.isValidCommand(command)) // syntax validation
// 	{
// 		throw (std::invalid_argument(rpl.reply(ERR_UNKNOWNCOMMAND, command["command"])));
// 	}
// 	if (!validator.validateCommand(command)) // semantic validation
// 	{
// 		ReplyType	errorCode = validator.getError();
// 		const std::vector<std::string>	&args = validator.getErrorArgs();
		
// 		throw (std::invalid_argument(rpl.reply(errorCode, args)));
// 	}
	
// 	return (Message(command));
// }

/*
Extracts and organize command data
	prefix if first token starts with ':'
	command
	parameters
	trailing message if ':' is found after the command
*/
std::map<std::string, std::string>	Parser::parseCommand(const std::vector<std::string> &tokens) const
{
	std::map<std::string, std::string>	command;
	
	if (tokens.empty())
		return (command);
	
	size_t	index = 0;
	
	// extract prefix if present
	if (tokens[index][0] == ':')
	{
		command["prefix"] = tokens[index].substr(1);
		index++;
	}
	
	// extract command
	if (index < tokens.size())
	{
		command["command"] = tokens[index];
		index++;
	}

	// extract parameters and trailing
	std::string	params;
	while (index < tokens.size())
	{
		if (tokens[index][0] == ':')
		{
			// everything after ':' is the trailing message
			std::string	trailing = tokens[index].substr(1);
			while (++index < tokens.size())
			{
				trailing += " " + tokens[index];
			}
			command["trailing"] = trailing;
			break ;
		}
		if (!params.empty())
			params += " ";
		params += tokens[index];
		++index;
	}
	command["params"] = params;

	return (command);
}
