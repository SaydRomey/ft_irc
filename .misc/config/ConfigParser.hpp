
#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <map>
# include <string>
# include <fstream>
# include <stdexcept>

class ConfigParser
{
	public:
		// load configuration from a file
		void	load(const std::string &filename);

		// retrieve a value (throws if not found)
		std::string	getValue(const std::string &section, const std::string &key) const;

		// check if a section or key exists
		bool	hasSection(const std::string &section) const;
		bool	hasKey(const std::string &section, const std::string &key) const;
	
	private:
		std::map<std::string, std::map<std::string, std::string> >	_configData;
		
		std::string	_trim(const std::string &str);
};

#endif // CONFIGPARSER_HPP
