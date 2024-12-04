/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_validator.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 23:55:18 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/01 23:58:58 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Validator.hpp"
#include <iostream>

void	test_validator(void)
{
	Validator validator;

	// valid command example
	std::map<std::string, std::string> validCommand;
	validCommand["command"] = "JOIN";
	validCommand["prefix"] = "SomeDude42";
	
	if (validator.isValidCommand(validCommand))
		std::cout << "Valid command" << std::endl;
	else
		std::cout << "Invalid command" << std::endl;

	// invalid command example
	std::map<std::string, std::string> invalidCommand;
	invalidCommand["command"] = "INVALIDCMD";
	invalidCommand["prefix"] = "SomeDude42";

	if (validator.isValidCommand(invalidCommand))
		std::cout << "Valid command" << std::endl;
	else
		std::cout << "Invalid command" << std::endl;
}
