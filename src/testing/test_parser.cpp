/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:17:25 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/15 23:09:24 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_test_header.hpp"
#include <iostream>

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
	
	std::cout << "\nTest " << (i + 1) << ": " << result << RESET << std::endl;
	std::cout << GRAYTALIC << msg << RESET << std::endl;
}

int	test_parser(void)
{
	Parser	parser;

	const std::string	validMessages[] = \
	{
		":nickname JOIN #channel :Hello world!",
		"PING :server1.example.com",
		":nickname PRIVMSG #channel :This is a test message",
		":server NOTICE * :Server maintenance scheduled",
		":nickname TOPIC #channel :New channel topic",
		":nickname    JOIN    #channel   :Hello    world!", // Extra spaces
		"   PING   :server1.example.com  ", // Command with extra spaces
		":nickname JOIN #channel\t:Hello\tworld!" // Tabs in the input
	};

	const std::string invalidMessages[] = \
	{
		":nickname :Hello world!", // Missing command
		":nickname JOIN channel :No # prefix", // Invalid channel
		":nickname PRIVMSG #channel :", // No trailing part
		": PRIVMSG #channel :Hello", // No prefix
		"", // Empty input
		":nickname PRIVMSG #channel", // No trailing part, valid command structure but missing trailing
		":invalid!nick PRIVMSG #channel :Invalid prefix format" // Invalid prefix format
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
	
	std::cout << UNDERLINE << "\nTesting invalid Messages:\n" << RESET << std::endl;
	i = 0;
	while (i < sizeof(invalidMessages) / sizeof(invalidMessages[0]))
	{
		try
		{
			Message	message = parser.parse(invalidMessages[i]);
			printMsg(invalidMessages[i], i);
			std::cout << RED << "Expected exception but none occurred.\n" << RESET << std::endl;
		}
		catch (const std::exception &e)
		{
			printMsg(invalidMessages[i], i, true);
			std::cout << GREEN << "Caught exception: " << RESET << e.what() << "\n" << std::endl;
		}
		++i;
	}
	
	return (0);
}
