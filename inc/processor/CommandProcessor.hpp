/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandProcessor.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 03:13:57 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/30 03:37:18 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDPROCESSOR_HPP
# define COMMANDPROCESSOR_HPP

#include "Aggregator.hpp"
#include "Parser.hpp"
#include "Reply.hpp"
#include "MessageValidator.hpp"
#include "CommandExecutor.hpp"
#include <string>
#include <vector>

class CommandProcessor
{
	public:
		CommandProcessor(CommandExecutor &executor);
		~CommandProcessor(void);

		// process raw input from a client and return replies
		std::vector<std::string>	processInput(int clientFd, const std::string &data);
	
	private:
		Aggregator _aggregator;
		Parser _parser;
		Reply _reply;
		MessageValidator _validator;
		CommandExecutor &_executor;

		std::string _handleCommand(int clientFd, const Message &message);
};

#endif // COMMANDPROCESSOR_HPP

/*
CommandProcessor handles input aggregation, parsing, validation, and reply generation.

Aggregate raw input using the Aggregator.
Parse and validate commands using the Parser and MessageValidator.
Route valid commands to the CommandExecutor.
Generate replies using the Reply class.
*/
