/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_tokenizer.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 13:19:00 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/21 13:27:07 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_test_header.hpp"

int	test_tokenizer(int argc, char *argv[])
{
	Parser	parser;
	
	if (argc < 2)
	{
		std::cout << "missing argument for 'test_tokenizer()" << std::endl;
		return (1);
	}

	try
	{
		std::cout << "Testing: " << argv[1] << std::endl;
		
		Message	message = parser.parse(argv[1]);
		std::cout << "  Command:\t" << message.getCommand() << std::endl;
		std::cout << "  Prefix:\t" << message.getPrefix() << std::endl;
		std::cout << "  Params:\t" << message.getParams() << std::endl;
		std::cout << "  Trailing:\t" << message.getTrailing() << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		return (1);
	}
	
	return (0);
}
