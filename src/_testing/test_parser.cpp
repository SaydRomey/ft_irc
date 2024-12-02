/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:17:25 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/02 00:45:04 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Parser.hpp"
#include "Validator.hpp"
#include "Message.hpp"
#include "_test_header.hpp"

/*
ideas to test next:

Commands with multiple colons (e.g., PING ::server1.example.com).
Messages with no trailing or parameters for commands that don't require them.
Messages with non-standard line endings (e.g., CRLF instead of LF)

*/

static void	printMsg(const std::string &input, size_t i, bool success = false)
{
	std::string	msg = !input.empty() ? std::string("\"" + input + "\"") : "[empty input]";
	std::string	result;
	
	if (success)
		result = std::string(std::string(GREEN) + "SUCCESS!");
	else
		result = std::string(std::string(RED) + "FAILURE!");
	
	std::cout << GRAYTALIC << msg << RESET << std::endl;
	std::cout << "Test " << (i + 1) << ": " << result << RESET << std::endl;
}

void	test_parser(void)
{
	Parser	parser;

	const std::string validMessages[] = {
		"PASS password123",
		"NICK SomeNick",
		"USER username * * :Real Name",
		":nickname JOIN #channel",
		":nickname PART #channel :Goodbye!",
		":nickname TOPIC #channel :New topic",
		":nickname MODE #channel +it",
		":nickname KICK #channel user :Reason for kick",
		":nickname INVITE user #channel",
		":nickname PRIVMSG #channel :Hello everyone!",
		"   :nickname     PRIVMSG    #channel :Hello     everyone!", // extra spaces
		":nickname JOIN #channel\t:Hello\tworld!", // tabs in the input
		":server NOTICE * :Server maintenance scheduled"
	};

	const std::string invalidMessages[] = {
		"", // Empty input
		"NICK ", // Missing nickname
		"USER username", // Incomplete USER command
		":nickname JOIN channel", // Missing # prefix for channel
		":nickname PART", // Missing channel name
		":nickname TOPIC", // Missing channel and topic
		":nickname MODE #channel +x", // Invalid mode
		":nickname KICK #channel", // Missing user and reason
		":nickname INVITE user", // Missing channel
		":nickname PRIVMSG", // Missing recipient and trailing
		":nickname PRIVMSG #channel", // Missing trailing
		":server NOTICE" // Missing target and message
	};
	
	size_t	i = 0;
	
	std::cout << UNDERLINE << "\nTesting valid Messages:\n" << RESET << std::endl;
	while (i < sizeof(validMessages) / sizeof(validMessages[0]))
	{
		try
		{
			Message	message = parser.parse(validMessages[i]);
			printMsg(validMessages[i], i, true);
			std::cout << "  Command:\t" << message.getCommand() << "\n";
			std::cout << "  Prefix:\t" << message.getPrefix() << "\n";
			std::cout << "  Params:\t" << message.getParams() << "\n";
			std::cout << "  Trailing:\t" << message.getTrailing() << "\n\n";
		}
		catch (const std::exception &e)
		{
			printMsg(validMessages[i], i);
			std::cout << " - " << e.what() << "\n" << std::endl;
		}
		++i;
	}
	
	// std::cout << UNDERLINE << "\nTesting invalid Messages:\n" << RESET << std::endl;
	// i = 0;
	// while (i < sizeof(invalidMessages) / sizeof(invalidMessages[0]))
	// {
	// 	try
	// 	{
	// 		Message	message = parser.parse(invalidMessages[i]);
	// 		printMsg(invalidMessages[i], i);
	// 		std::cout << RED << "Expected exception but none occurred.\n" << RESET << std::endl;
	// 	}
	// 	catch (const std::exception &e)
	// 	{
	// 		printMsg(invalidMessages[i], i, true);
	// 		std::cout << GREEN << "Caught exception: " << RESET << e.what() << "\n" << std::endl;
	// 	}
	// 	++i;
	// }
}
