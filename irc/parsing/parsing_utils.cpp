
#include "parsing_utils.hpp"

/*
Creates a std::vector<std::string> with variadic argument
** Can be used with either 1, 2, 3 or 4 arguments
*/
t_vecStr	makeArgs(const std::string &arg1, const std::string &arg2, const std::string &arg3, const std::string &arg4)
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

/* ************************************************************************** */ // Tokens

/*
Returns a tokenized std::vector<std::string>
** If unspecified, default ' ' will be used as delimiter
** Set 'includeEmptyTokens' to true to keep empty tokens in vector

ex:
(if using it to tokenize multiple channel keys in JOIN command)

	tokenize ("key1,,key3", ',');		-> {"key1"}, {"key3"}
	tokenize ("key1,,key3", ',', true);	-> {"key1"}, {""}, {"key3"}
*/
t_vecStr	tokenize(const std::string &input, char delimiter, bool includeEmptyTokens)
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
Checks if the number of tokens in 'params' is valid compared to 'expectedNum'

'type' parameter options (using VerificationType enum):
	AT_LEAST:	true if param count >= expectedNum.
	EXACTLY:	true if param count == expectedNum.
	AT_MOST:	true if param count <= expectedNum.
*/
bool	hasValidNumberOfParams(const std::string &params, VerificationType type, int expectedNum)
{
	if (params.empty())
		return (false);
	
	int	paramNum = countTokens(params);

	// debug_param_tokens(params, type, expectedNum, paramNum); // Debug

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

/*
Checks for the presence of a comma (',') in 'input'
*/
bool	hasMultipleEntries(const std::string &input)
{
	return (input.find(',') != std::string::npos);
}

/* ************************************************************************** */ // String Formatting

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
Reformats a string by replacing multiple space char (' ') by a single space
** (Also removes the "\r\n" sequence or any standalone '\r' or '\n')
** If used on the IRC message input, will also normalize the trailing part...

ex: 

	":nickname   JOIN    #channel   :Hello    world!"

becomes:

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
std::string	crlf(const std::string &str) { return (str + "\r\n"); }

/* ************************************************************************** */ // Debug

/*
Debug output
Prints the content of a  std::vector<std::string>
*/
void	printVec(const t_vecStr &vec, const std::string &msg, bool printIndex)
{
	if (!msg.empty())
		std::cout << msg << std::endl;

	size_t	i = 0;
	while (i < vec.size())
	{
		std::cout << "  ";
		if (printIndex)
			std::cout << "[" << i << "]: ";
		std::cout << vec[i] << std::endl;
		++i;
	}

	if (vec.empty())
		std::cout << "  <empty vector>" << std::endl;

	std::cout << std::endl;
}


/*
Debug output
Prints the content of a  std::map<std::string, std::string>
*/
void	printMap(const t_mapStrStr &map, const std::string &msg)
{
	if (!msg.empty())
		std::cout << msg << std::endl;

	t_mapStrStr::const_iterator	it = map.begin();
	while (it != map.end())
	{
		std::cout << "  Key:   " << it->first << "\n";
		std::cout << "  Value: " << it->second << "\n" << std::endl;
		++it;
	}
}

/*
Debug output
Prints the contents of a std::vector<std::pair<std::string, std::string> >
*/
void	printPairs(const t_vecPairStrStr &pairs, const std::string &msg)
{
	if (!msg.empty())
		std::cout << msg << std::endl;

	size_t	i = 0;
	while (i < pairs.size())
	{
		std::cout << "  First:  " << pairs[i].first << "\n";
		if (!pairs[i].second.empty())
		{
			std::cout << "  Second: " << pairs[i].second << "\n";
		}
		else
		{
			std::cout << "  Second: <none>" << "\n";
		}
		std::cout << std::endl;
		++i;
	}
}

/*
Debug output
Usable in 'hasValidNumberOfParams()'
*/
void	debug_param_tokens(const std::string &params, VerificationType type, int expectedNum, int paramNum)
{
	std::cout << YELLOW << "** Debug: hasValidNumberOfParams **" << RESET << std::endl;
	
	std::string typeStr;

	switch (type)
	{
		case AT_LEAST:
			typeStr = "AT LEAST";
			break ;
		case EXACTLY:
			typeStr = "EXACTLY";
			break ;
		case AT_MOST:
			typeStr = "AT MOST";
			break ;
		default:
			typeStr = "*Bad VerificationType enum*";
	}
	std::cout << "  Validating params: " << params << "\n";
	std::cout << "  Token count: " << paramNum << "\n";
	std::cout << "  Expected:    " << typeStr << " " << expectedNum << " params" << std::endl;
}

