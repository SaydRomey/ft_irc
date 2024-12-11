/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 18:41:48 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/06 17:16:55 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_UTILS_HPP
# define PARSING_UTILS_HPP

# include <algorithm>
# include <iostream>
# include <map>
# include <sstream>
# include <vector>

# define RESET		"\033[0m"
# define BOLD		"\033[1m"
# define ITALIC		"\033[3m"
# define UNDERLINE	"\033[4m"
# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define YELLOW		"\033[33m"
# define BLUE		"\033[34m"
# define PURPLE		"\033[35m"
# define CYAN		"\033[36m"
# define ORANGE		"\033[38;5;208m"
# define GRAYTALIC	"\033[3;90m"

std::vector<std::string>	tokenize(const std::string &input, char delimiter = ' ');
std::vector<std::string>	makeArgs(const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "");

std::string trim(const std::string &str);
std::string	normalizeInput(const std::string &input);

void	printMap(const std::map<std::string, std::string> &parsedCommand, const std::string &msg = "");

#endif // PARSING_UTILS_HPP
