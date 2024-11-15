/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:38:15 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/15 12:56:29 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDPARSER_HPP
# define COMMANDPARSER_HPP

# include "Tokenizer.hpp"
# include <string>
# include <vector>
# include <map>

class CommandParser
{
	public:
		CommandParser(void);
		std::map<std::string, std::string>	parseCommand(const std::vector<std::string> &tokens);
	
	private:
		std::string	_parsePrefix(const std::string &token);
};

#endif // COMMANDPARSER_HPP
