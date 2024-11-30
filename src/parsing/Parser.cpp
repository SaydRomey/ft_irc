/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:11:39 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/30 02:28:35 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "_parsing_utils.hpp"
#include <stdexcept>

Parser::Parser(void) {}

/*
Use a single entry point (Parser::parse) to handle:

Tokenizing input
Parsing commands
Validating input

Tokenizer: Splits the input.
CommandParser: Extracts and organizes command data.
MessageValidator: Validates command-specific details.

Return a Message object for valid inputs.
*/
Message	Parser::parse(const std::string &rawInput)
{
	std::string	trimmedInput = trim(rawInput);
	std::vector<std::string>	tokens = _tokenizer.tokenize(trimmededInput);
	std::map<std::string, std::string>	parsedCommand = _commandParser.parseCommand(tokens);
	
	if (!_validator.isValidCommand(parsedCommand))
	{
		throw (std::invalid_argument("Invalid command"));
	}
	
	if (parsedCommand.count("prefix") && !_validator.isValidNickname(parsedCommand["prefix"]))
	{
		throw (std::invalid_argument("Invalid nickname format"));
	}
		
	return (Message(parsedCommand));
}
