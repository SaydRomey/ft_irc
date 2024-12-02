/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_tokenizer.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 13:19:00 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/02 00:09:36 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "Parser.hpp"
#include <iostream>

void	test_tokenizer(int argc, char *argv[])
{
	std::cout << "Testing: " << argv[1] << std::endl;
	if (argc < 2)
	{
		std::cout << "missing argument for 'test_tokenizer()" << std::endl;
		return ;
	}
	try
	{
		Parser	parser;
		
		Message	message = parser.parse(argv[1]);
		
		std::cout << "  Command:\t" << message.getCommand() << std::endl;
		std::cout << "  Prefix:\t" << message.getPrefix() << std::endl;
		std::cout << "  Params:\t" << message.getParams() << std::endl;
		std::cout << "  Trailing:\t" << message.getTrailing() << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		return ;
	}
}
