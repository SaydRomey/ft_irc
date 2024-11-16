/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:41:20 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/15 23:05:26 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandParser.hpp"
#include <iostream> // for debug

#include <algorithm>
#include <cctype>

std::string trim(const std::string &str)
{
	size_t	start = str.find_first_not_of(" \t");
	size_t	end = str.find_last_not_of(" \t");

	if (start == std::string::npos || end == std::string::npos)
		return (""); // String is entirely whitespace

	return (str.substr(start, end - start + 1));
}


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
	if (i < tokens.size())
		command["params"] = trim(tokens[i++]);

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
