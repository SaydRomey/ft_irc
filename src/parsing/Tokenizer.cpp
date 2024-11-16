/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:31:37 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/15 23:05:40 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tokenizer.hpp"
#include <sstream>

#include <regex> // for normalization

std::string	normalizeInput(const std::string &input)
{
	// Replace multiple spaces or tabs with a single space
	std::string	normalized = std::regex_replace(input, std::regex("[ \t]+"), " ");
	
	return (normalized);
}

Tokenizer::Tokenizer(void) {}

std::vector<std::string>	Tokenizer::tokenize(const std::string &input)
{
	std::string	normalizedIntput = normalizeInput(input);
	std::vector<std::string>	tokens;
	size_t	prefixPos = normalizedIntput.find(" :");
	
	std::string	mainPart = normalizedIntput.substr(0, prefixPos);
	_split(mainPart, ' ', tokens);

	if (prefixPos != std::string::npos)
	{
		std::string	trailing = normalizedIntput.substr(prefixPos + 2); // extract trailing part
		tokens.push_back(trailing); // push even if it's empty
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
