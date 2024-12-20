
#include "ConfigParser.hpp"
#include <iostream>

#define CONFIG_FILE	"server_config.ini"

int	test_config_parser(void)
{
	ConfigParser	config;

	try
	{
		config.load(CONFIG_FILE);

		// get server settings
		std::string	ip = config.getValue("server", "ip");
		int	port = std::atoi(config.getValue("server", "port").c_str());
		std::string	password = config.getVale("server", "password");

		std::cout << "Server IP: " << ip << std::endl;
		std::cout << "Server Port: " << port << std::endl;
		std::cout << "Server Password: " << password << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		return (1);
	}
	return (0);
}
