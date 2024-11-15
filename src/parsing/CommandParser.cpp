/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:41:20 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/15 13:31:48 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandParser.hpp"

CommandParser::CommandParser(void) {}

std::map<std::string, std::string>	CommandParser::parseCommand(const std::vector<std::string> &tokens)
{
	std::map<std::string, std::string>	command;
	
	if (tokens.empty())
		return (command);
	
	size_t	i = 0;
	if (tokens[i][0] == ':')
	{
		command["prefix"] = _parsePrefix(tokens[i++]);
	}
	if (i < tokens.size())
	{
		command["command"] = tokens[i++];
	}
	if (i < tokens.size())
	{
		command["params"] = tokens[i++];
	}
	if (i < tokens.size())
	{
		command["trailing"] = tokens[i];
	}
	return (command);
}

std::string	CommandParser::_parsePrefix(const std::string &tokens)
{
	return (tokens.substr(1)); // remove the leading ':'
}
