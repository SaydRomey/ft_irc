/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:52:54 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/05 20:33:35 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

// # include "Parser.hpp"

# include <iostream>
# include <string>
# include <map>

/*	TODO: the << overload should be a proper string *!!

*!!! simplify the thing: Message constructor with string arg, puts in through Parser

*/

class Message
{
	public:
		// Message();
		// ~Message();
		
		// Message(const std::string &input); // ** <- THIS !
		
		Message(const std::map<std::string, std::string> &parsedCommand);
		// const std::string&	getMessageStr(void) const;
		const std::string&	getPrefix(void) const;
		const std::string&	getCommand(void) const;
		const std::string&	getParams(void) const;
		const std::string&	getTrailing(void) const;
	
	private:
		std::string	_prefix;
		std::string	_command;
		std::string	_params;
		std::string	_trailing;
		// std::string	_messageStr; // the full message
};

std::ostream&	operator<<(std::ostream &out, const Message &message);

#endif // MESSAGE_HPP
