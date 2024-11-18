/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:11:39 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/18 12:49:40 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "_parsing_utils.hpp"
#include <iostream> // for debug

Parser::Parser(void) {}

Message	Parser::parse(const std::string &rawInput)
{
	std::string	trimmedInput = trim(rawInput);
	std::vector<std::string>	tokens = _tokenizer.tokenize(trimmedInput);
	std::map<std::string, std::string>	parsedCommand = _commandParser.parseCommand(tokens);
	
	printMap(parsedCommand, "** Parsed Command:");
	
	// Validate command presence and validity
	if (!_validator.isValidCommand(parsedCommand))
	{
		throw (std::invalid_argument("Invalid command in input"));
	}
	
	// Validate nickname if prefix exists
	if (parsedCommand.count("prefix") && !_validator.isValidNickname(parsedCommand["prefix"]))
	{
		throw (std::invalid_argument("Invalid nickname format"));
	}

	// Validate trailing part for commands that require it
	if ((parsedCommand["command"] == "PRIVMSG" || parsedCommand["command"] == "NOTICE") && \
		(!parsedCommand.count("trailing") || parsedCommand["trailing"].empty()))
	{
		throw (std::invalid_argument("Trailing part is required"));
	}
	
	// Validate channel for JOIN command)
	if (parsedCommand["command"] == "JOIN" && !_validator.isValidChannel(parsedCommand["params"]))
	{
		// logError("Invalid channel format: " + parsedCommand["params"]);
		throw (std::invalid_argument("Invalid channel format"));
	}
	
	if (parsedCommand["command"] == "MODE" && !isValidModeCommand(parsedCommand["params"]))
	{
		throw (std::invalid_argument("Invalid Mode command"));
	}
	
	return (Message(parsedCommand));
}

/*
2.3.1 Message format in Augmented BNF

   The protocol messages must be extracted from the contiguous stream of
   octets.  The current solution is to designate two characters, CR and
   LF, as message separators.  Empty messages are silently ignored,
   which permits use of the sequence CR-LF between messages without
   extra problems.

   The extracted message is parsed into the components <prefix>,
   <command> and list of parameters (<params>).

	The Augmented BNF representation for this is:

	message    =  [ ":" prefix SPACE ] command [ params ] crlf
	prefix     =  servername / ( nickname [ [ "!" user ] "@" host ] )
	command    =  1*letter / 3digit
	params     =  *14( SPACE middle ) [ SPACE ":" trailing ]
			   =/ 14( SPACE middle ) [ SPACE [ ":" ] trailing ]

	nospcrlfcl =  %x01-09 / %x0B-0C / %x0E-1F / %x21-39 / %x3B-FF
					; any octet except NUL, CR, LF, " " and ":"
	middle     =  nospcrlfcl *( ":" / nospcrlfcl )
	trailing   =  *( ":" / " " / nospcrlfcl )

	SPACE      =  %x20        ; space character
	crlf       =  %x0D %x0A   ; "carriage return" "linefeed"
*/
