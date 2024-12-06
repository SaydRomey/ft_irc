/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 18:44:16 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/05 20:30:27 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing_utils.hpp"

/*
Create a vector of string tokens using ' ' as delimiter
*/
std::vector<std::string>	tokenize(const std::string &input)
{
	std::vector<std::string>	tokens;

	std::stringstream	ss(input);
	std::string			token;
	char				delimiter = ' ';

	while (std::getline(ss, token, delimiter))
	{
		if (!token.empty()) // ignore empty tokens caused by consecutive delimiters
			tokens.push_back(token);
	}
	return (tokens);
}

/*
Helper function to build argument vector with variadic argument...
[prefix] [cmd] [param] [trailing]
*/
std::vector<std::string>	makeArgs(const std::string &arg1, const std::string &arg2, const std::string &arg3, const std::string &arg4)
{
	std::vector<std::string>	args;
	
	if (!arg1.empty())
		args.push_back(arg1);
	if (!arg2.empty())
		args.push_back(arg2);
	if (!arg3.empty())
		args.push_back(arg3);
	if (!arg4.empty())
		args.push_back(arg4);
	
	return (args);
}


/*
Remove leading and trailing whitespace
*/
std::string trim(const std::string &str)
{
	size_t	start = str.find_first_not_of(" \t");
	size_t	end = str.find_last_not_of(" \t");

	if (start == std::string::npos || end == std::string::npos)
		return (""); // String is entirely whitespace

	return (str.substr(start, end - start + 1));
}

/*	** check if trim() is needed ..

Reformat a string by replacing multiple space char (' ') by a single space
ex: 
	":nickname   JOIN    #channel   :Hello    world!"
would become 
	":nickname JOIN #channel :Hello world!"
*/
std::string normalizeInput(const std::string &input)
{
	std::string	normalized;
	normalized.reserve(input.size());

	bool	previousWasSpace = false;

	std::string::const_iterator it = input.begin();
	while (it != input.end())
	{
		// Replace multiple spaces or tabs with a single space
		if (*it == ' ' || *it == '\t')
		{
			if (!previousWasSpace)
			{
				normalized += ' ';
				previousWasSpace = true;
			}
		}
		// Remove carriage returns and line breaks
		else if (*it != '\r' && *it != '\n')
		{
			normalized += *it;
			previousWasSpace = false;
		}
		++it;
	}

	return (normalized);
}

/*
Print map<string, string> content:
"  [key]: [value]"

*/
void	printMap(const std::map<std::string, std::string> &parsedCommand, const std::string &msg)
{
	// if (!DEBUG)
		// return ;

	std::cout << msg << std::endl;

	std::map<std::string, std::string>::const_iterator	it = parsedCommand.begin();
	while (it != parsedCommand.end())
	{
		std::cout << "  " << it->first << ": " << it->second << std::endl;
		++it;
	}
}


