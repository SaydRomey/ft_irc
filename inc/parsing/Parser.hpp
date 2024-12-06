/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:08:44 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/05 23:31:27 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

// # include "Message.hpp"
// # include "Validator.hpp"

# include <map>
# include <string>
# include <vector>

/*	TOCHECK:

change isValidCommand to isValidSyntax ?
change validateCommand to isValidSemantic >

*/

class Parser
{
	public:
		Parser(void);
		~Parser(void);
		
		// Message	parse(const std::string &input);
		std::map<std::string, std::string>	parseCommand(const std::vector<std::string> &tokens) const;
	
	private:
};

#endif // PARSER_HPP
