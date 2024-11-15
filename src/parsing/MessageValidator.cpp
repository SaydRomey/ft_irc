/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageValidator.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:48:44 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/15 12:55:57 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageValidator.hpp"
#include <cctype>

MessageValidator::MessageValidator(void) {}

bool	MessageValidator::isValidCommand(const std::map<std::string, std::string> &command)
{
	return (!command.at("command").empty());
}

bool	MessageValidator::isValidNickname(const std::string &nickname)
{
	return (_isAlphanumeric(nickname) && !nickname.empty());
}

bool	MessageValidator::isValidChannel(const std::string &channel)
{
	return (channel[0] == '#' && _isAlphanumeric(channel.substr(1)));
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
