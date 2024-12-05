/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_reply.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 02:48:34 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/04 21:57:28 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Reply.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

void	test_reply(void)
{
	Reply	rpl;

	try
	{
		std::string	nickname = "HomeBoy";
		
		std::string	welcomeMsg = rpl.reply(RPL_WELCOME, nickname, nickname);
		std::cout << welcomeMsg << std::endl;

		std::cout << rpl.reply(ERR_NEEDMOREPARAMS, "PRIVMSG") << std::endl;
		std::cout << rpl.reply(ERR_ALREADYREGISTERED) << std::endl;

	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}
