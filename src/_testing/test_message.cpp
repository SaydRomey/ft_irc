/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_message.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 22:45:48 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/12 14:11:32 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "_test_header.hpp"
#include <iostream>

static void printMsg(const Message &message, size_t i, bool success)
{
	std::string	result;
	
	if (success)
		result = std::string(std::string(GREEN) + "SUCCESS!");
	else
		result = std::string(std::string(RED) + "FAILURE!");


	std::cout << GRAYTALIC << "\"" << message.getInput() << "\"" << RESET << std::endl;
	std::cout << "Test " << (i + 1) << ": " << result << RESET << std::endl;

	if (success)
		std::cout << message << "\n" << std::endl;
	else
		std::cout << "  Reply: " << message.getReply() << "\n" << std::endl;
}

void	runTests(const std::string messages[], size_t count, const std::string &testType, bool expectSuccess)
{
	std::cout << UNDERLINE << "\nTesting " << testType << " Messages:\n" << RESET << std::endl;

	size_t	i = 0;
	
	while (i < count)
	{
		try
		{
			Message	msg(messages[i]);
			bool	success = msg.getReply().empty() == expectSuccess; // comparing reply state with expected result
			
			printMsg(msg, i, success);
		}
		catch (const std::exception &e)
		{
			std::cout << "Unexpected exception: " << e.what(); // std::endl;
		}
		++i;
		if (expectSuccess)
			std::cout << CYAN << "/* ************************************* */\n" << RESET << std::endl;
		else
			std::cout << ORANGE << "/* ************************************* */\n" << RESET << std::endl;
	}
}

void	test_message(void)
{
	const std::string	validMessages[] = {
		"PASS password123",
		"NICK SomeNick",
		"USER username * * :Real Name",
		":nickname JOIN #channel",
		":nickname JOIN #channel,#channel2,#channel3,#channel4 pass1,,pass3,*",
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
		":nickname MODE #channel +unknown_mode", // Invalid mode
		":nickname MODE #channel +itZ", // Invalid mode after valid mode
		":nickname KICK #channel", // Missing user and reason
		":nickname INVITE user", // Missing channel
		":nickname PRIVMSG", // Missing recipient and trailing
		":nickname PRIVMSG #channel", // Missing trailing
		":server NOTICE" // Missing target and message
	};
	
	size_t	validCount = sizeof(validMessages) / sizeof(validMessages[0]);
	size_t	invalidCount = sizeof(invalidMessages) / sizeof(invalidMessages[0]);

	runTests(validMessages, validCount, "Valid", true);
	runTests(invalidMessages, invalidCount, "Invalid", false);	
}
