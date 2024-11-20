/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:49:03 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/15 16:22:50 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "ErrorManager.hpp"

// ErrorManager::ErrorManager(void)
// {
// 	initializeErrorCodes();
// }

// /*
// Set up error code-to-message mappings
// */
// void	ErrorManager::initErrorCodes(void)
// {
// 	_errorMessages["ERR_NICKNAMEINUSE"] = "Nickname is already in use";
// 	_errorMessages["ERR_UNKOWNCOMMAND"] = "Unknown command";
// 	_errorMessages["ERR_NOTREGISTERED"] = "You have not registered";
// 	_errorMessages["ERR_NEEDMOREPARAMS"] = "Not enough parameters";
// 	_errorMessages["ERR_CHANNELISFULL"] = "Channel is full";
// 	// _errorMessages[""] = "";
// 	// TODO: add more IRC-specific error codes later...
// }

// /*
// Retrieve error message by code
// */
// const std::string& ErrorManager::getErrorMsg(const std::string &errorCode) const
// {
// 	std::map<std::string, std::string>::const_iterator	it = _errorMessages.find(errorCode);

// 	if (it == _errorMessages.end())
// 	{
// 		throw (ErrorException("Unknown error code: ") + errorCode);
// 	}
// 	return (it->second);
// }

// void ErrorManager::logError(const std::string &errorCode, const std::string &details) const
// {
// 	std::cerr << "[ERROR] " << errorCode << ": " << getErrorMsg(errorCode);
// 	if (!details.empty())
// 	{
// 		std::cerr << " (" << details >> ")";
// 	}
// 	std::cerr << std::endl;
// }

// /*
// Format error response for clients
// */
// std::string	ErrorManager::formatErrorResponse(const std::string &errorCode, const std::string &additionalInfo) const
// {
// 	std::string	response = ":" + errorCode + " " + getErrorMsg(errorCode);

// 	if (!additionalInfo.empty())
// 	{
// 		response += " - " + additionalInfo;
// 	}
// 	return (response);
// }

// ErrorManager::ErrorException::ErrorException(const std::string &msg) : _message(message) {}

// const char* ErrorManager::ErrorException::what() const throw()
// {
// 	return (_message.c_str());
// }
