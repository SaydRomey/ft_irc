/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_tokenizer.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 13:19:00 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/05 23:30:02 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "_test_header.hpp"
#include <iostream>

void	test_tokenizer(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Error: Missing argument for 'test_tokenizer()'" << std::endl;
		return ;
	}

	std::cout << "Testing: " << argv[1] << std::endl;

	try
	{		
		Message	message(argv[1]);
		
		std::cout << "  Input:\t" << message.getInput() << std::endl;
		std::cout << "  Command:\t" << message.getCommand() << std::endl;
		std::cout << "  Prefix:\t" << message.getPrefix() << std::endl;
		std::cout << "  Params:\t" << message.getParams() << std::endl;
		std::cout << "  Trailing:\t" << message.getTrailing() << std::endl;

		if (!message.getReply().empty())
		{
			std::cout << "  Reply:\t" << message.getReply() << std::endl;
		}
	}
	catch (std::exception &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		return ;
	}
}
