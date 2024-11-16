/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageValidator.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:48:44 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/15 23:04:42 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageValidator.hpp"
#include <cctype>
#include <set>
#include <iostream> // for debug

MessageValidator::MessageValidator(void) {}

bool	MessageValidator::isValidCommand(const std::map<std::string, std::string> &command)
{
	static const std::string	commandsArray[] = {"JOIN", "PING", "PRIVMSG", "NOTICE", "TOPIC"};
	static const std::set<std::string>	validCommands(commandsArray, commandsArray + sizeof(commandsArray) / sizeof(commandsArray[0]));
	
	if (!command.count("command") || command.at("command").empty())
	{
		std::cout << "** Debug: Command field is missing or empty." << std::endl;
		return (false);
	}

	// TODO: add any additional rules for valid commands here (e.g., valid command names)
	if (validCommands.find(command.at("command")) == validCommands.end())
	{
		std::cout << "** Debug: Command is not a valid IRC command: " << command.at("command") << std::endl;
		return (false);
	}
	
	std::cout << "** Debug: Command is valid: " << command.at("command") << std::endl;
	return (true);
}

bool	MessageValidator::isValidNickname(const std::string &nickname)
{
	if (nickname.empty() || !std::isalpha(nickname[0]))
		return (false); // must start with a letter
	
	size_t	i = 1;
	while (i < nickname.size())
	{
		if (!std::isalnum(nickname[i]) && nickname[i] != '-' && nickname[i] != '_')
		{
			return (false); // only allow letters, digits, dashes and underscores
		}
		++i;
	}
	return (true);
}

bool	MessageValidator::isValidChannel(const std::string &channel)
{
	return (!channel.empty() && channel[0] == '#' && _isAlphanumeric(channel.substr(1)));
}

bool	MessageValidator::_isAlphanumeric(const std::string &str)
{
	size_t	i = 0;

	while (i < str.size())
	{
		if (!std::isalnum(str[i]))
			return (false);
		++i;
	}
	return (true);
}
