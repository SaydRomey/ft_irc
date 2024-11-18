/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _parsing_utils.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 22:52:27 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/18 15:43:57 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PARSING_UTILS_HPP
# define _PARSING_UTILS_HPP

# include <string>
# include <algorithm>
# include <cctype>
# include <map>
#include <iostream>
#include <sstream>

#include <regex> // for normalization

std::string trim(const std::string &str);
std::string	normalizeInput(const std::string &input);

// log functions
void	printMap(const std::map<std::string, std::string> &parsedCommand, const std::string &msg);



#endif // _PARSING_UTILS_HPP
