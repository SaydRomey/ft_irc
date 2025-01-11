/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandExecutor.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 03:25:12 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/30 03:52:17 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDEXECUTOR_HPP
# define COMMANDEXECUTOR_HPP

#include "Reply.hpp"
#include "Message.hpp"
#include <string>

class CommandExecutor
{
	public:
		CommandExecutor(void);
		~CommandExecutor(void);

		// execute a command and return the resulting reply
		std::string	execute(int clientFd, const Message &message);
	
	private:
		Reply _reply;
	
		std::string	_handleJoin(int clientFd, const Message &message);
		std::string	_handlePrivMsg(int clientFd, const Message &message);
		std::string	_handleUnknownCommand(const Message &message);
};

#endif // COMMANDEXECUTOR_HPP

/*
CommandExecutor focuses on executing commands and managing server behavior.

Implement the business logic for commands.
Interact with other components like channels, users, and the server core
*/