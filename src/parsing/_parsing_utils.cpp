
#include "_parsing_utils.hpp"

std::string trim(const std::string &str)
{
	size_t	start = str.find_first_not_of(" \t");
	size_t	end = str.find_last_not_of(" \t");

	if (start == std::string::npos || end == std::string::npos)
		return (""); // String is entirely whitespace

	return (str.substr(start, end - start + 1));
}

std::string	normalizeInput(const std::string &input)
{
	// Replace multiple spaces or tabs with a single space
	std::string	normalized = std::regex_replace(input, std::regex("[ \t]+"), " ");
	// Remove unnecessary carriage returns or line breaks
	std::string	normalized = std::regex_replace(input, std::regex("\r\n"), "");
	
	return (normalized);
}

//

std::vector<std::string>	splitParams(const std::string &params, char delimiter)
{
	std::vector<std::string>	result;
	std::stringstream	ss(params);
	std::string	item;

	while (std::getline(ss, item, delimiter))
	{
		result.push_back(item);
	}
	return (result);
}

std::string	joinParams(const std::vector<std::string> &params)
{
	std::string	result;
	size_t	i = 0;

	while (i < params.size())
	{
		if (i > 0)
			result += " ";
		result += params[i];
		++i;
	}
	return (result);
}
