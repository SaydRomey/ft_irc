/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_reply.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 02:48:34 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/29 12:02:35 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_test_header.hpp"

void	test_reply(void)
{
	Reply	reply;

	try
	{
		std::string	welcome = reply.welcome("HomeBoy");
		std::string	nicknameError = reply.nicknameInUse("HomeBoy");
		std::string	privateMessage = reply.privateMessage("senderPerson", "recieverPerson", "Hello my friend!");

		std::cout << welcome << std::endl;
		std::cout << nicknameError << std::endl;
		std::cout << privateMessage << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}
