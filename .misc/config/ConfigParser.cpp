
#include "ConfigParser.hpp"

/*
Load the configuration file
*/
void	ConfigParser::load(const std::string &filename)
{
	std::ifstream	file(filename.c_str());
	if (!file.is_open())
		throw (std::runtime_error("Failed to open configuration file: " + filename));

	std::string	line, currentSection;
	while (std::getline(file, line))
	{
		line = _trim(line);

		// ignore empty lines and comments
		if (line.empty() || line[0] == '#' || line[0] == ';')
			continue ;

		// section header
		if (line[0] == '[' && line[line.size() - 1] == ']')
		{
			currentSection = line.substr(1, line.size() - 2);
			currentSection = _trim(currentSection);
			continue ;
		}

		// key-value pair
		size_t	delimiterPos = line.find('=');
		if (delimiterPos == std::string::npos)
			throw (std::runtime_error("Invalid line in configuration file: " + line));

		std::string	key = _trim(line.substr(0, delimiterPos));
		std::string	value = _trim(line.substr(delimiterPos + 1 ));

		_configData[currentSection][key] = value;
	}
}

/*
Retrieve a value
*/
std::string	ConfigParser::getValue(const std::string &section, const std::string &key) const
{
	std::map<std::string, std::map<std::string, std::string> >::const_iterator	sectionIt = _configData.find(section);
	if (sectionIt == _configData.end())
		throw (std::runtime_error("Section not found: " + section));

	std::map<std::string, std::string>::const_iterator	keyIt = sectionIt->second.find(key);
	if (keyIt == sectionIt->second.end())
		throw (std::runtime_error("Key not found in section [" + section + "]: " + key));

	return (keyIt->second);
}

bool	ConfigParser::hasSection(const std::string &section) const
{
	return (_configData.find(section) != _configData.end());
}

bool	ConfigParser::hasKey(const std::string &section, const std::string &key) const
{
	std::map<std::string, std::map<std::string, std::string> >::const_iterator	sectionIt = _configData.find(section);
	if (sectionIt == _configData.end())
		return (false);

	return (sectionIt->second.find(key) != sectionIt->second.end());

}

std::string	ConfigParser::_trim(const std::string &str)
{
	size_t	first = str.find_first_not_of(" \t");
	size_t	last = str.find_last_not_of(" \t");

	if (first == std::string::npos)
		return ("");
	else
		return (std.substr(first, last - first + 1));
}
