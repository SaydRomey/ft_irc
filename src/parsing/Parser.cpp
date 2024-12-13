/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:11:39 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/13 04:00:05 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "parsing_utils.hpp"
#include "Reply.hpp"
#include <stdexcept>
# include <utility>				// std::make_pair()

Parser::Parser(void) {}
Parser::~Parser(void) {}

/*
Extracts and organize command data
	prefix if first token starts with ':'
	command
	parameters
	trailing message if ':' is found after the command
*/
std::map<std::string, std::string>	Parser::parseCommand(const std::string &input) const
{
	std::map<std::string, std::string>	command;
	std::vector<std::string>			tokens = tokenize(input);

	command["prefix"] = "";
	command["command"] = "";
	command["params"] = "";
	command["trailing"] = "";
	
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

/*	** assumes params has multiple entries
	
Creates a vector of pairs of strings, unsing ',' as a delimiter,
Pairs tokens from first param with tokens from second param
(returns empty vector<string> if )
* Used to tokenize an input of multiple channels and keys,
formated like so:
	"#channel1,#channel2,#channel3 pass1,pass2,pass3"
or
	"#channel1,#channel2,#channel3 pass1,,pass3"

(key inputs can be empty)
*/
std::vector<std::pair<std::string, std::string> > Parser::parseChannelsAndKeys(const std::string &params) const
{
	
	// tokenize params into two parts
	std::vector<std::string>	paramTokens = tokenize(params);
	if (paramTokens.size() != 2) // > 2)
		throw (std::invalid_argument("Invalid params: must contain exactly one space-separated list of channels and keys"));
	
	std::vector<std::string>	channelTokens = tokenize(paramTokens[0], ',');
	std::vector<std::string>	keyTokens = tokenize(paramTokens[1], ',');

	std::vector<std::pair<std::string, std::string> >	result;
	result.reserve(channelTokens.size()); // to avoid reallocation

	size_t	i = 0;
	while (i < channelTokens.size())
	{
		std::string	key = ""; // default to an empty key
		
		if (i < keyTokens.size())
		{
			key = keyTokens[i];
			if (key == "*")
				key = ""; // if key is exactly "*", treat as empty field (TODO: check with teammates...)
		}
		
		result.push_back(std::make_pair(channelTokens[i], key));
		++i;
	}
	return (result);
}
