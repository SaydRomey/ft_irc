/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:31:37 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/15 12:56:15 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tokenizer.hpp"
#include <sstream>

Tokenizer::Tokenizer(void) {}

std::vector<std::string>	Tokenizer::tokenize(const std::string &input)
{
	std::vector<std::string>	tokens;
	size_t	prefixPos = input.find(" :");
	std::string	mainPart = input.substr(0, prefixPos);

	_split(mainPart, ' ', tokens);
	if (prefixPos != std::string::npos)
	{
		tokens.push_back(input.substr(prefixPos + 2));
	}
	return (tokens);
}

void	Tokenizer::_split(const std::string &str, char delimiter, std::vector<std::string> &tokens)
{
	std::stringstream	ss(str);
	std::string	item;

	while (std::getline(ss, item, delimiter))
	{
		tokens.push_back(item);
	}
}
