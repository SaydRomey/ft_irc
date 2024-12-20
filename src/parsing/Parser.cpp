/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:11:39 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/20 14:43:12 by cdumais          ###   ########.fr       */
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
	** (not anymore ?) ** prefix if first token starts with ':'
	command
	parameters
	trailing message if ':' is found after the command
*/
t_mapStrStr	Parser::parseCommand(const std::string &input) const
{
	t_mapStrStr					command;
	std::vector<std::string>	tokens = tokenize(input);

	command["prefix"] = "";
	command["command"] = "";
	command["params"] = "";
	command["trailing"] = "";

	// Debug: print tokens
	std::cout << YELLOW << "**DEBUG: tokens after tokenize(input):" << RESET << std::endl;
	size_t	i = 0;
	while (i < tokens.size())
	{
		std::cout << "  Token " << i << ": " << tokens[i] << std::endl;
		++i;
	} 
	
	if (tokens.empty())
		return (command);
	
	size_t	index = 0;

	// // ignore prefix if present ?
	// if (tokens[index][0] == ':')
	// 	index++;
	
	// extract prefix if present
	if (tokens[index][0] == ':')
	{
		command["prefix"] = tokens[index].substr(1);
		index++;
	}
	
	// extract command
	if (index < tokens.size() && tokens[index][0] != ':')
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

	// Debug: print parsed command structure
	std::cout << YELLOW << "**DEBUG: parsed command structure:" << RESET << std::endl;
	
	t_mapStrStr::const_iterator	it = command.begin();
	while (it != command.end())
	{
		std::cout << "  key: " << it->first << ", Value: " << (it->second.empty() ? "<empty>" : it->second) << std::endl;
		++it;
	}

	return (command);
}

/*	**need to decide if we skip or flag a missing channel in multiparams "#chan1,,#chan3,#chan4"

** assumes params has multiple entries and has at most 2 params
** (checked in Message::_processJoinCommand())
	
Creates a vector of pairs of strings, unsing ',' as a delimiter,
Pairs tokens from first param with tokens from second param

* Used to tokenize an input of multiple channels and keys,
formated like so:
	"#channel1,#channel2,#channel3 pass1,pass2,pass3"
or
	"#channel1,#channel2,#channel3 pass1,,pass3"

* Key inputs can be empty ('*' is considered as meaning empty)
* If more keys than channels, the remaining unpaired keys are ignored
*/
t_vecPairStrStr	Parser::parseChannelsAndKeys(const std::string &params) const
{
	std::vector<std::string>	paramTokens = tokenize(params);
	std::vector<std::string>	channelTokens = tokenize(paramTokens[0], ',', true);
	std::vector<std::string>	keyTokens = tokenize(paramTokens[1], ',', true);

	t_vecPairStrStr	result;
	result.reserve(channelTokens.size());

	size_t	i = 0;
	while (i < channelTokens.size())
	{
		std::string	key = "";
		
		if (i < keyTokens.size())
		{
			key = keyTokens[i];
			if (key == "*")
				key = "";
		}
		
		result.push_back(std::make_pair(channelTokens[i], key));
		++i;
	}
	return (result);
}
/*
	std::vector<std::string>	channelTokens = tokenize(params.substr(0, params.find(' ')), ',', true);
	std::vector<std::string>	keyTokens = tokenize(params.substr(params.find(' ') + 1), ',', true);
*/
