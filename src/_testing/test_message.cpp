/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_message.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 22:45:48 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/20 14:57:22 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "Reply.hpp"
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
			std::cout << GREEN << "/* ************************************* */\n" << RESET << std::endl;
		else
			std::cout << RED << "/* ************************************* */\n" << RESET << std::endl;
	}
}

void	test_message(void)
{
	const std::string	validMessages[] = {
		"PASS password123",
		"NICK SomeNick",
		"USER username * * :Real Name",
		"JOIN #channel",
		"JOIN #channel1,#channel2,#channel3,#channel4 pass1,,pass3,*",
		"PART #channel :Goodbye!",
		"TOPIC #channel :New topic",
		"MODE #channel +it",
		"KICK #channel user :Reason for kick",
		"INVITE user #channel",
		"PRIVMSG #channel :Hello everyone!",
		"       PRIVMSG    #channel :Hello     everyone!", // extra spaces
		"JOIN #channel\t:Hello\tworld!", // tabs in the input
		":server NOTICE target :Server maintenance scheduled"
	};

	const std::string invalidMessages[] = {
		"", // Empty input
		"NICK ", // Missing nickname
		"USER username", // Incomplete USER command
		"JOIN channel", // Missing # prefix for channel
		"PART", // Missing channel name
		"TOPIC", // Missing channel and topic
		"MODE #channel +unknown_mode", // Invalid mode
		"MODE #channel +itZ", // Invalid mode after valid mode
		"KICK #channel", // Missing user and reason
		"INVITE user", // Missing channel
		"PRIVMSG", // Missing recipient and trailing
		"PRIVMSG #channel", // Missing trailing
		":server NOTICE" // Missing target and message
	};
	
	size_t	validCount = sizeof(validMessages) / sizeof(validMessages[0]);
	size_t	invalidCount = sizeof(invalidMessages) / sizeof(invalidMessages[0]);

	runTests(validMessages, validCount, "Valid", true);
	runTests(invalidMessages, invalidCount, "Invalid", false);
}

void	test_pseudo_replies(void)
{
	std::cout << PURPLE << "/* ************************************* */\n" << RESET << std::endl;
	std::cout << UNDERLINE << "\nTesting pseudo replies\n" << RESET << std::endl;

	std::cout << GRAYTALIC << "\":<clientNickname> JOIN :<channelName>\"" << RESET << std::endl;
	std::cout << joinMsg("HonorableGuest", "#general") << std::endl;

	std::cout << GRAYTALIC << "\":<clientNickname> PART <channelName> [:<partingMessage>]\"" << RESET << std::endl;
	std::cout << partMsg("HonorableGuest", "#general") << std::endl;
	std::cout << partMsg("HonorableGuest", "#general", "Goodbye!") << std::endl;

	std::cout << GRAYTALIC << "\":<kickerNickname> KICK <channelName> <targetNickname> [:<reason>]\"" << RESET << std::endl;
	std::cout << kickMsg("UptightTim", "#general", "SleepyHead") << std::endl;
	std::cout << kickMsg("UptightTim", "#general", "SleepyHead", "Too sleepy...") << std::endl;
	
	std::cout << GRAYTALIC << "\":<senderNickname> INVITE <targetNickname> :<channelName>\"" << RESET << std::endl;
	std::cout << inviteMsg("HonorableHost", "HonorableGuest", "#general") << std::endl;
}
