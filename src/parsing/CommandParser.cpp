/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:41:20 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/17 19:08:08 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandParser.hpp"
#include "_parsing_utils.hpp"
#include <iostream> // for debug

CommandParser::CommandParser(void) {}

std::map<std::string, std::string>	CommandParser::parseCommand(const std::vector<std::string> &tokens)
{
	std::map<std::string, std::string>	command;
	
	if (tokens.empty())
		return (command);
	
	size_t	i = 0;
	
	// Extract prefix if present
	if (tokens[i][0] == ':')
		command["prefix"] = trim(_parsePrefix(tokens[i++]));

	// Extract command
	if (i < tokens.size() && !tokens[i].empty())
		command["command"] = trim(tokens[i++]);
	else
		command["command"] = ""; // explicitly empty if no command is found

	// Remaining tokens: params and trailing
	// if (i < tokens.size())
	// 	command["params"] = trim(tokens[i++]);
	
	// *! modification (to test) to parse commands with multiple arguments
	// Extract all remaining tokens as params
	std::string	params;
	while (i < tokens.size() && tokens[i][0] != ':')
	{
		if (!params.empty())
			params += " ";
		params += tokens[i++];
	}
	command["params"] = trim(tokens[i++]);
		
	// Extract trailing
	if (i < tokens.size())
		command["trailing"] = trim(tokens[i]);

	// // Debugging to verify parsed command
	// std::cout << "** Parsed Command Debugging:" << std::endl;
	// std::map<std::string, std::string>::iterator	it = command.begin();
	// while (it != command.end())
	// {
	// 	std::cout << "  " << it->first << ": " << it->second << std::endl;
	// 	++it;
	// }

	return (command);
}

std::string	CommandParser::_parsePrefix(const std::string &tokens)
{
	return (tokens.substr(1)); // remove the leading ':'
}
