/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 18:44:16 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/20 14:34:18 by cdumais          ###   ########.fr       */
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
** Set 'includeEmptyTokens' to true to keep empty tokens in vector
** ex., if using it to tokenize multiple channel keys in JOIN command:
	tokenize ("key1,,key3", ',');		-> {"key1"}, {"key3"}
	tokenize ("key1,,key3", ',', true);	-> {"key1"}, {""}, {"key3"}
*/
std::vector<std::string>	tokenize(const std::string &input, char delimiter, bool includeEmptyTokens)
{
	std::vector<std::string>	tokens;
	std::stringstream			ss(input);
	std::string					token;

	while (std::getline(ss, token, delimiter))
	{
		if (includeEmptyTokens || !token.empty())
			tokens.push_back(token);
	}
	return (tokens);
}

/*
Returns the number of tokens in a string, using 'delimiter' or default ' '
*/
int	countTokens(const std::string &str, char delimiter)
{
	if (str.empty())
		return (0);

	std::istringstream	stream(str);
	std::string			token;
	
	int	count = 0;
	while (std::getline(stream, token, delimiter))
	{
		++count;
	}
	return (count);
}

/*
Checks if the number of tokens in 'params' is valid compared to 'expectedNum'.
Modes (using VerificationType enum):
	AT_LEAST:	true if param count >= expectedNum.
	EXACTLY:	true if param count == expectedNum.
	AT_MOST:	true if param count <= expectedNum.
*/
bool	hasValidNumberOfParams(const std::string &params, int expectedNum, VerificationType type)
{
	if (params.empty())
		return (false);
	
	int	paramNum = countTokens(params);

	// Debug: trace token count
	std::string typeStr;
	switch (type)
	{
		case AT_LEAST:
			typeStr = "AT_LEAST";
			break ;
		case EXACTLY:
			typeStr = "EXACTLY";
			break ;
		case AT_MOST:
			typeStr = "AT_MOST";
			break ;
		default:
			typeStr = "*bad type*";
	}
	std::cout << YELLOW << "**Debug: hasValidNumberOfParams\n" \
	<< "Validating params: " << params \
	<< " | Token count: " << paramNum \
	<< " | Expected: " << typeStr << " " << expectedNum \
	<< " params" << RESET << std::endl;
	
	switch (type)
	{
		case AT_LEAST:
			return (paramNum >= expectedNum);
		case EXACTLY:
			return (paramNum == expectedNum);
		case AT_MOST:
			return (paramNum <= expectedNum);
		default:
			return (false);
	}
}

bool	hasMultipleEntries(const std::string &param)
{
	return (param.find(',') != std::string::npos);
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

