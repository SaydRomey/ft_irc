/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_error.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:21:18 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/15 16:24:18 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "_test_header.hpp"

// int	test_log_error(void)
// {
// 	ErrorManager	errorManager;

// 	try
// 	{
// 		errorManager.logError("ERR_UNKNOWNCOMMAND", "Tried to execute unsupported command XYZ");
// 	}
// 	catch (const ErrorManager::ErrorException &e)
// 	{
// 		std::cerr << "Error exception caught: " << e.what() << std::endl;
// 	}
// 	return (0);
// }

// int	test_send_error_response(void)
// {
// 	ErrorManager	errorManager;

// 	try
// 	{
// 		std::string	response = errorManager.formatErrorResponse("ERR_NICKNAMEINUSE", "Attempted nickname: Marvin");
// 		std::cout << "Send to client: " << response << std::endl;
// 	}
// 	catch (const ErrorManager::ErrorException &e)
// 	{
// 		std::cerr << "Error exception caught: " << e.what() << std::endl;
// 	}
// 	return (0);
// }

// int	test_custom_err(void)
// {
// 	ErrorManager	errorManager;

// 	try
// 	{
// 		throw (ErrorManager::ErrorException("Custom error for invalid state"));
// 	}
// 	catch (const ErrorManager::ErrorException &e)
// 	{
// 		std::cerr << "Error exception caught: " << e.what() << std::endl;
// 	}
// 	return (0);
// }
