/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:08:44 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/02 00:12:32 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include "Message.hpp"
# include "Validator.hpp"
# include <map>
# include <string>
# include <vector>

# include <algorithm> // trim

# include <iostream> // print map
# include <sstream> // print map

class Parser
{
	public:
		Parser(void);
		~Parser(void);
		
		Message	parse(const std::string &input);
	
	private:
		std::vector<std::string>	_tokenize(const std::string &input);
		std::map<std::string, std::string>	_parseCommand(const std::vector<std::string> &tokens);
};

std::string trim(const std::string &str);
std::string	normalizeInput(const std::string &input);

// tmp log and debug functions
void	printMap(const std::map<std::string, std::string> &parsedCommand, const std::string &msg = "");


#endif // PARSER_HPP
