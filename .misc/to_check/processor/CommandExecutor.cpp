/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandExecutor.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 03:25:33 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/30 03:47:27 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandExecutor.hpp"

CommandExecutor::CommandExecutor(void) {}
CommandExecutor::~CommandExecutor(void) {}

std::string	CommandExecutor::execute(int clientFd, const Message &message)
{
	const std::string &command = message.getCommand();

	if (command == "JOIN")
	{
		return (_handleJoin(clientFd, message));
	}
	else if (command == "PRIVMSG")
	{
		return (_handlePrivMsg(clientFd, message));
	}
	else
		return (_handleUnknownCommand(message));
}
	
std::string	CommandExecutor::_handleJoin(int clientFd, const Message &message)
{
	const std::string	&channelName = message.getParams();

	if (channelName.empty())
	{
		return (_reply.generateReply(Reply::ERR_NEEDMOREPARAMS, makeArgs("JOIN")));
	}
	if (!_validator.isValidChannel(channelName))
	{
		return (_reply.generateReply(Reply::ERR_NOSUCHCHANNEL, makeArgs(channelName)));
	}

	// return (_reply.generateReply(Reply::RPL_JOIN_SUCCESS, makeArgs(channelName)));
	return (_reply.joinSuccess(channelName));
}

std::string	CommandExecutor::_handlePrivMsg(int clientFd, const Message &message)
{
	const std::string	&reciever = message.getParams();
	const std::string	&text = message.getTrailing();

	if (reciever.empty() || text.empty())
	{
		return (_reply.generateReply(Reply::ERR_NEEDMOREPARAMS, makeArgs("PRIVMSG")));
	}

	// return (_reply.generateReply(Reply::RPL_PRIVATE_MSG, makeArgs("Client" + std::to_string(clientFd), reciever, text)));
	return (_reply.privateMessage("Client" + std::to_string(clientFd), receiver, text));
}

std::string	CommandExecutor::_handleUnknownCommand(const Message &message)
{
	return (_reply.generateReply(Reply::ERR_UNKNOWNCOMMAND, makeArgs(message.getCommand())));
}

/* example usage:

#include "CommandProcessor.hpp"
#include "CommandExecutor.hpp"
#include <iostream>

int	main(void)
{
    CommandExecutor		executor;
    CommandProcessor	processor(executor);

    int	clientFd = 1;
    std::string	incomingData = "JOIN #general\r\nPRIVMSG #general :Hello everyone!\r\n";

    std::vector<std::string>	replies = processor.processInput(clientFd, incomingData);

	size_t	i = 0;
	while (i < replies.size())
	{
        std::cout << "Reply: " << replies[i] << std::endl;
		++i;
    }

    return (0);
}
*/
