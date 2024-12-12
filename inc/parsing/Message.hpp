/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:52:54 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/10 23:13:17 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "Parser.hpp"
# include "Validator.hpp"
# include "Reply.hpp"

# include <iostream>
# include <map>
# include <string>
// # include <utility> // For std::pair ? or std::make_pair()

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

		// const std::vector<std::pair<std::string, std::string> >	&getChannelsAndKeys(void) const; // ? if tokenized access is required, else implement tools to be used..
		
	private:
		std::string							_input;
		std::map<std::string, std::string>	_parsedMessage;
		std::string							_reply;
		
		// std::vector<std::pair<std::string, std::string> >	_channelsAndKeys; // ? if we want to store channel/key pairs for edge case of "CMD #ch1,#ch2 key1,key2"
	
		Parser		_parser;
		Validator	_validator;
		// Reply		_rpl; // less overhead if we create a Reply instance too often.. (TOCHECK)
		// Reply		&_rpl; // (if we pass it as an external single Reply object (in constructor) (TOCHECK))

		void	_processInput(const std::string &input);
};

std::ostream	&operator<<(std::ostream &out, const Message &message);

#endif // MESSAGE_HPP
