/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:17:25 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/15 13:39:16 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_test_header.hpp"
#include <iostream>

#define VALID_MSG ""

int	test_parser(void)
{
	Parser	parser;
	
	try
	{
		Message	message = parser.parse(":nickname JOIN #channel :Hello world!");
		
		std::cout << "Command: " << message.getCommand() << "\n";
		std::cout << "Prefix: " << message.getPrefix() << "\n";
		std::cout << "Params: " << message.getParams() << "\n";
		std::cout << "Trailing: " << message.getTrailing() << "\n";
	}
	catch (const std::exception& e)
	{
		std::cerr << "Parsing error: " << e.what() << "\n";
	}

	return (0);
}
