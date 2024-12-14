/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:08:44 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/13 20:21:03 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <map>
# include <string>
# include <vector>

typedef std::vector<std::pair<std::string, std::string> >	t_vecPairStrStr;
typedef std::map<std::string, std::string>					t_mapStrStr;

class Parser
{
	public:
		Parser(void);
		~Parser(void);
		
		t_mapStrStr			parseCommand(const std::string &input) const;
		t_vecPairStrStr		parseChannelsAndKeys(const std::string &params) const;
	
	private:
};

#endif // PARSER_HPP
