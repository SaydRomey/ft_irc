/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:39:06 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/18 17:57:49 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

/* ************************************************************************** */ // Tokenizer.cpp

#include <sstream>

std::vector<std::string>	Tokenizer::tokenize(const std::string &input)
{
	std::vector<std::string>	tokens;
	
	_split(input, ' ', tokens);

	return (tokens);
}

void	Tokenizer::_split(const std::string &str, char delimiter, std::vector<std::string> &tokens)
{
	std::stringstream	ss(str);
	std::string	token;

	while (std::getline(ss, token, delimiter))
	{
		if (!token.empty())
			tokens.push_back(token);
	}
}

/* ************************************************************************** */ // 

std::map<std::string, std::string>	CommandParser::parseCommand(const std::vector<std::string> &tokens)
{
	std::map<std::string, std::string>	commands;

	if (tokens.empty())
		return (commands);

	size_t	index = 0;

	// extract prefix if it starts with ':'
	if (tokens[0][0] == ':')
	{
		commands["prefix"] = _parsePrefix(tokens[index]);
		index++;
	}

	// extract command
	if (index < tokens.size())
	{
		commands["command"] = tokens[index];
		index++;
	}
	
	// extract parameters and trailing message
	std::string params;
	std::string trailing;
	while (index < tokens.size())
	{
		if (tokens[index][0] == ':')
		{
			trailing = tokens[index].substr(1);
			break ;
		}
		if (!params.empty())
			params += " ";
		params += tokens[index];
		++index;
	}
	commands["params"] = params;
	commands["trailing"] = trailing;

	return (commands);
}

std::string	CommandParser::_parsePrefix(const std::string &token)
{
	return (token.substr(1));
}

/* ************************************************************************** */


/* ************************************************************************** */


/* ************************************************************************** */

