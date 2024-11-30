/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandProcessor.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 03:14:09 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/30 04:10:10 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandProcessor.hpp"

CommandProcessor::CommandProcessor(CommandExecutor &executor) : _executor(executor) {}
CommandProcessor::~CommandProcessor(void) {}

std::vector<std::string>	CommandProcessor::processInput(int clientFd, const std::string &data)
{
	std::vector<std::string>	completeCommands = _aggregator.processData(clientFd, data);
	std::vector<std::string>	replies;

	// parse and handle each complete command
	std::vector<std::string>::iterator	it = completeCommands.begin();
	while (it != completeCommands.end())
	{
		try
		{
			Message	message = _parser.parse(*it); // parse command
			replies.push_back(_handleCommand(clientFd, message)); // execute and reply
		}
		catch (const std::exception &e)
		{
			replies.push_back(_reply.generateReply(Reply::ERR_NEEDMOREPARAMS, makeArgs(*it)));
			// replies.push_back(_reply.needMoreParams(*it));
		}
		++it;
	}
	
	return (replies);
}

std::string	CommandProcessor::_handleCommand(int clientFd, const Message &message)
{
	const std::string &command = message.getCommand();

	// validate the command
	if (!_validator.isValidCommand(message.getParams()))
	{
		return (_reply.generateReply(Reply::ERR_UNKNOWNCOMMAND, makeArgs(command)));
		// return (_reply.needMoreParams(command));
	}

	// route the command to the executor
	return (_executor.execute(clientFd, message));
}
