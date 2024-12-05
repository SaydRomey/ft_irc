/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply_ideas.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 20:29:20 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/03 20:36:41 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

/*
// add private map attribute to store templates in Reply
std::map<std::string, std::string> _templates;

// call loadTemplates() in constructor
Reply::Reply()
{
	loadTemplates("ReplyTemplates.conf");
}

*/

void	Reply::loadTemplates(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		throw (std::runtime_error("Failed to open templates file: " + filename));
	}

	std::string line;
	size_t lineNumber = 0;

	while (std::getline(file, line))
	{
		++lineNumber;

		// Trim whitespace from both ends
		size_t start = line.find_first_not_of(" \t");
		size_t end = line.find_last_not_of(" \t");
		if (start == std::string::npos || line[start] == '#')
		{
			// Skip empty lines or lines that start with a comment
			continue ;
		}
		line = line.substr(start, end - start + 1);

		// Find the separator '='
		size_t separator = line.find('=');
		if (separator == std::string::npos)
		{
			std::ostringstream oss;
			oss << "Invalid format at line " << lineNumber << ": missing '=' separator";
			throw (std::runtime_error(oss.str()));
		}

		// Parse the key and value
		std::string key = line.substr(0, separator);
		std::string value = line.substr(separator + 1);

		// Insert into the map
		if (_templates.find(key) != _templates.end())
		{
			std::ostringstream oss;
			oss << "Duplicate key at line " << lineNumber << ": " << key;
			throw (std::runtime_error(oss.str()));
		}
		_templates[key] = value;
	}

	file.close();
}

/*
(simplified version, does not allow empty lines or comments, no error checking for duplicates, bad format (key=value) or leading/trailing whitespace..)
*/
// void Reply::loadTemplates(const std::string &filename)
// {
// 	std::ifstream file(filename.c_str());
// 	if (!file.is_open())
// 	{
// 		throw (std::runtime_error("Failed to open templates file: " + filename));
// 	}

// 	std::string line;
// 	while (std::getline(file, line))
// 	{
// 		size_t separator = line.find('=');
// 		if (separator != std::string::npos)
// 		{
// 			std::string key = line.substr(0, separator);
// 			std::string value = line.substr(separator + 1);
// 			_templates[key] = value;
// 		}
// 	}
// }
