/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:31:37 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/17 20:28:18 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tokenizer.hpp"
#include "_parsing_utils.hpp"
#include <sstream>

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

/* to test, multiple commands in a single message separated by \r\n


std::vector<std::string>	Tokenizer::tokenize(const std::string &input)
{
	std::vector<std::string>	commands;
	size_t	start = 0, end;

	while ((end = input.find("\r\n", start)) != std::string::npos)
	{
		command.push_back(input.substr(start, end - start));
		start = end + 2;
	}
	if (start < input.size())
	{
		commands.push_back(input.substr(start));
	}
	return (command);
}
*/

void	Tokenizer::_split(const std::string &str, char delimiter, std::vector<std::string> &tokens)
{
	std::stringstream	ss(str);
	std::string	item;

	while (std::getline(ss, item, delimiter))
	{
		tokens.push_back(item);
	}
}
