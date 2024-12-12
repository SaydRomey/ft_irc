/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 18:44:16 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/12 13:57:38 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing_utils.hpp"

/*	**TODO: change description to more generic..

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
Creates a vector of string tokens from 'input' using 'delimiter' as delimiter
*/
std::vector<std::string>	tokenize(const std::string &input, char delimiter)
{
	std::vector<std::string>	tokens;

	std::stringstream	ss(input);
	std::string			token;

	while (std::getline(ss, token, delimiter))
	{
		if (!token.empty()) // ignore empty tokens caused by consecutive delimiters
			tokens.push_back(token);
	}
	return (tokens);
}


bool	hasMultipleEntries(const std::string &param)
{
	return (param.find(',') != std::string::npos);
}

/*
** assumes params contains multiple entries (previously checked in higher level with 'hasMultipleEntries()')
*/
// std::vector<std::pair<std::string, std::string> >	processChannelsAndKeys(const std::string &params)
// {
// 	// check anyways?
// 	if (!hasMultipleEntries(params))
// 		throw (std::invalid_argument("Invalid params: Missing or malformed entries for channels and keys."));
	
// 	// tokenize params into two parts
// 	std::vector<std::string>	paramTokens = tokenize(params);
// 	if (paramTokens.size() != 2)
// 		throw (std::invalid_argument("Invalid params in processChannelsAndKeys: Expected two entries (channels and keys)."));
	
// 	std::string	channels = paramTokens[0];
// 	std::string	keys = paramTokens[1];

// 	return (pairChannelsAndKeys(channels, keys));
// }



// std::vector<std::pair<std::string, std::string> >	pairChannelsAndKeys(const std::string &channels, const std::string &keys)
// {
// 	std::vector<std::string>	channelTokens = tokenize(channels, ',');
// 	std::vector<std::string>	keyTokens = tokenize(keys, ',');

// 	std::vector<std::pair<std::string, std::string> >	result;
// 	result.reserve(channelTokens.size()); // to avoid reallocation

// 	size_t	i = 0;
// 	while (i < channelTokens.size())
// 	{
// 		std::string	key = ""; // default to an empty key
		
// 		if (i < keyTokens.size())
// 		{
// 			key = keyTokens[i];
			
// 			if (key == "*")
// 			{
// 				key = ""; // if key is exactly "*", treat as empty field (TODO: check with teammates...)
// 			}
// 		}
		
// 		result.push_back(std::make_pair(channelTokens[i], key));
// 		++i;
// 	}
// 	return (result);
// }

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

/*
Reformat a string by replacing multiple space char (' ') by a single space
** (Also removes the "\r\n" sequence or any standalone '\r' or '\n')
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
Returns formatted string with appropriate line-ending sequence
*/
std::string	crlf(const std::string &str)
{
	// #define CRLF "\r\n"
	// return (str + CRLF);
	
	return (str + "\r\n");
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

void	printChannelKeyPairs(const std::vector<std::pair<std::string, std::string> > &pairs)
{
	// Print each pair in a readable format
	size_t	i = 0;
	while (i < pairs.size())
	{
		std::cout << "Channel: " << pairs[i].first;
		if (!pairs[i].second.empty())
		{
			std::cout << ", Key: " << pairs[i].second;
		}
		else
		{
			std::cout << ", Key: <none>";
		}
		std::cout << std::endl;
		++i;
	}
}

