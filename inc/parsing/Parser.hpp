/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:08:44 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/13 02:07:01 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <map>
# include <string>
# include <vector>

// typedef std::vector<std::pair<std::string, std::string> >	t_vecPairStrStr; // ?

class Parser
{
	public:
		Parser(void);
		~Parser(void);
		
		std::map<std::string, std::string>	parseCommand(const std::string &input) const;
		std::vector<std::pair<std::string, std::string> >	parseChannelsAndKeys(const std::string &params) const;
		// t_vecPairStrStr	parseChannelsAndKeys(const std::string &params) const;
	
	private:
};

#endif // PARSER_HPP
