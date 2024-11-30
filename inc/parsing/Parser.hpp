/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:08:44 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/30 02:18:49 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include "Tokenizer.hpp"
# include "CommandParser.hpp"
# include "MessageValidator.hpp"
# include "Message.hpp"
# include <string>

class Parser
{
	public:
		Parser(void);
		Message	parse(const std::string &rawInput);
	
	private:
		Tokenizer			_tokenizer;
		CommandParser		_commandParser;
		MessageValidator	_validator;
};

#endif // PARSER_HPP
