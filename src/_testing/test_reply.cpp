/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_reply.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 02:48:34 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/05 21:17:05 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Reply.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

# define RESET		"\033[0m"
# define UNDERLINE	"\033[4m"
# define GRAYTALIC	"\033[3;90m"

void	printRpl(const std::string &reply, const std::string &numeric="[reply]")
{
	std::cout << GRAYTALIC << "\n[" << numeric << "]" << RESET << std::endl;
	std::cout << "  " << reply << "\n" << std::endl;
}

void	test_reply(void)
{
	std::cout << UNDERLINE << "\n** Testing Reply **" << RESET << std::endl;

	try
	{
		Reply	rpl;

		std::string	nickname = "HomeBoy";
		std::string	sender = "senderPerson";
		std::string	reciever = "recieverPerson";
		
		printRpl(rpl.reply(RPL_WELCOME, nickname, nickname), "RPL_WELCOME (RplType)");
		printRpl(rpl.reply(static_cast<ReplyType>(001), nickname, nickname), "RPL_WELCOME (static_cast)");
		printRpl(rpl.reply(001, nickname, nickname), "RPL_WELCOME (int -> overload)");

		printRpl(rpl.reply(433, nickname), "ERR_NICKNAMEINUSE");
		printRpl(rpl.reply(461, "PRIVMSG"), "ERR_NEEDMOREPARAMS");
		printRpl(rpl.reply(462), "ERR_ALREADYREGISTERED");
		
	}
	catch (const std::exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
}
