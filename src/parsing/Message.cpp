/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:57:01 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/15 14:32:18 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

Message::Message(const std::map<std::string, std::string> &parsedCommand)
{
	// _prefix = parsedCommand.find("prefix") != parsedCommand.end() ? parsedCommand.at("prefix") : "";
	// _command = parsedCommand.find("command") != parsedCommand.end() ? parsedCommand.at("command") : "";
	// _params = parsedCommand.find("params") != parsedCommand.end() ? parsedCommand.at("params") : "";
	// _trailing = parsedCommand.find("trailing") != parsedCommand.end() ? parsedCommand.at("trailing") : "";

	if (parsedCommand.find("prefix") != parsedCommand.end())
		_prefix = parsedCommand.at("prefix");
	else
		_prefix = "";

	if (parsedCommand.find("command") != parsedCommand.end())
		_command = parsedCommand.at("command");
	else
		_command = "";

	if (parsedCommand.find("params") != parsedCommand.end())
		_params = parsedCommand.at("params");
	else
		_params = "";

	if (parsedCommand.find("trailing") != parsedCommand.end())
		_trailing = parsedCommand.at("trailing");
	else
		_trailing = "";
}

const std::string&	Message::getPrefix(void) const
{
	return (_prefix);
}

const std::string&	Message::getCommand(void) const
{
	return (_command);
}

const std::string&	Message::getParams(void) const
{
	return (_params);
}

const std::string&	Message::getTrailing(void) const{
	return (_trailing);
}
