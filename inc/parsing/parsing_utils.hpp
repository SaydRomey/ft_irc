/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 18:41:48 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/05 20:30:33 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_UTILS_HPP
# define PARSING_UTILS_HPP

# include <algorithm>
# include <iostream>
# include <map>
# include <sstream>
# include <vector>

std::vector<std::string>	tokenize(const std::string &input);
std::vector<std::string>	makeArgs(const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "");

std::string trim(const std::string &str);
std::string	normalizeInput(const std::string &input);

void	printMap(const std::map<std::string, std::string> &parsedCommand, const std::string &msg = "");

#endif // PARSING_UTILS_HPP
