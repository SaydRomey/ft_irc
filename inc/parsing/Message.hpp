/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:52:54 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/06 19:59:59 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*	*TOCHECK:
	
	should the getters be inline to access parsed data ?
		inline const std::string &getPrefix() const { return _parsedMessage["prefix"]; }
*/

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "Parser.hpp"
# include "Validator.hpp"
# include "Reply.hpp"

# include <iostream>
# include <map>
# include <string>

class Message
{
	public:
		// Message(void);
		// ~Message(void);
		// Message(const Message &other);
		// Message& operator=(const Message &other);
		
		Message(const std::string &input);
		
		const std::string	&getInput(void) const;
		const std::string	&getPrefix(void) const;
		const std::string	&getCommand(void) const;
		const std::string	&getParams(void) const;
		const std::string	&getTrailing(void) const;
		const std::string	&getReply(void) const;
		
	private:
		std::string	_input;
		std::map<std::string, std::string>	_parsedMessage;
		std::string	_reply;
	
		Parser		_parser;
		Validator	_validator;
		// Reply		_rpl;

		void	_processInput(const std::string &input);
};

std::ostream	&operator<<(std::ostream &out, const Message &message);

#endif // MESSAGE_HPP
