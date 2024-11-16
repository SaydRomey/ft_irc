/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:41:38 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/15 16:22:42 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #ifndef ERRORMANAGER_HPP
// # define ERRORMANAGER_HPP

// # include <string>
// # include <map>
// # include <stdexcept>
// # include <iostream>

// class ErrorManager
// {
// 	public:
// 		ErrorManager(void);

// 		void	initErrorCodes(void);

// 		const std::string&	getErrorMsg(const std::string &errorCode) const;
		
// 		void	logError(const std::string &errorCode, const std::string &details = "") const;
		
// 		std::string	formatErrorResponse(const std::string &errorCode, const std::string &additionalInfo = "") const;
		
// 		class ErrorException : public std::exception
// 		{
// 			public:
// 				explicit ErrorException(const std::string &message);
// 				virtual const char* what() const throw();
			
// 			private:
// 				std::string	_msg;
// 		};
	
// 	private:
// 		std::map<std::string, std::string>	_errorMessages;
// };

// #endif // ERRORMANAGER_HPP
