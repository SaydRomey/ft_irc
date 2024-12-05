/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:11:39 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/04 20:27:28 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Reply.hpp"
#include <stdexcept>

Parser::Parser(void) {}
Parser::~Parser(void) {}


/*
Use a single entry point (Parser::parse) to handle:

Tokenizing: split the input
Parsing: extract and organize command data
Validating: validate command-specific details

Returns a Message object for valid inputs.
Throws a formatted reply for invalid inputs.
*/
Message	Parser::parse(const std::string &input)
{
	std::vector<std::string>	tokens = _tokenize(input);
	std::map<std::string, std::string>	command = _parseCommand(tokens);
	
	Validator	validator;
	Reply		rpl;

	if (!validator.isValidCommand(command)) // syntax validation
	{
		throw (std::invalid_argument(rpl.unknownCommand(command["command"])));
	}
	if (!validator.validateCommand(command)) // semantic validation
	{
		throw (std::invalid_argument(rpl.needMoreParams(command["command"])));
	}
	
	return (Message(command));
}

/*
Format the input string as a vector of string tokens
*/
std::vector<std::string>	Parser::_tokenize(const std::string &input)
{
	std::vector<std::string>	tokens;

	std::stringstream	ss(input);
	std::string			token;
	char				delimiter = ' ';

	while (std::getline(ss, token, delimiter))
	{
		if (!token.empty()) // ignore empty tokens caused by consecutive delimiters
			tokens.push_back(token);
	}
	return (tokens);
}

/*
Extracts and organize command data
	prefix if first token starts with ':'
	command
	parameters
	trailing message if ':' is found after the command
*/
std::map<std::string, std::string>	Parser::_parseCommand(const std::vector<std::string> &tokens)
{
	std::map<std::string, std::string>	command;
	
	if (tokens.empty())
		return (command);
	
	size_t	index = 0;
	if (tokens[index][0] == ':')
	{
		command["prefix"] = tokens[index].substr(1);
		index++;
	}
	if (index < tokens.size())
	{
		command["command"] = tokens[index];
		index++;
	}

	std::string	params;
	while (index < tokens.size())
	{
		if (tokens[index][0] == ':')
		{
			command["trailing"] = tokens[index].substr(1);
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

/* ************************************************************************** */

std::string trim(const std::string &str)
{
	size_t	start = str.find_first_not_of(" \t");
	size_t	end = str.find_last_not_of(" \t");

	if (start == std::string::npos || end == std::string::npos)
		return (""); // String is entirely whitespace

	return (str.substr(start, end - start + 1));
}

/*
Reformat a string by replacing multiple space char (' ') by a single space
ezample, 
	":nickname   JOIN    #channel   :Hello    world!"
would become 
	":nickname JOIN #channel :Hello world!"
*/
std::string normalizeInput(const std::string &input)
{
	std::string	normalized;
	normalized.reserve(input.size());

	bool	previousWasSpace = false;

	std::string::const_iterator it = input.begin();
	while (it != input.end())
	{
		// Replace multiple spaces or tabs with a single space
		if (*it == ' ' || *it == '\t')
		{
			if (!previousWasSpace)
			{
				normalized += ' ';
				previousWasSpace = true;
			}
		}
		// Remove carriage returns and line breaks
		else if (*it != '\r' && *it != '\n')
		{
			normalized += *it;
			previousWasSpace = false;
		}
		++it;
	}

	return (normalized);
}

/* ************************************************************************** */

void	printMap(const std::map<std::string, std::string> &parsedCommand, const std::string &msg)
{
	// if (!DEBUG)
		// return ;

	std::cout << msg << std::endl;

	std::map<std::string, std::string>::const_iterator	it = parsedCommand.begin();
	while (it != parsedCommand.end())
	{
		std::cout << "  " << it->first << ": " << it->second << std::endl;
		++it;
	}
}
