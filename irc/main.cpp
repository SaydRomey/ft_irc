
#include "Server.hpp"
#include "utils.hpp"

void	serverStatePrint(bool state)
{
	std::cout << BOLD << UNDERLINE << "ircserv" << RESET;
	if (state == true)
	{
		std::cout << "\t\t" << GREEN << "Server is runing..." << RESET;
	}
	else if (state == false)
	{
		std::cout << "\t\t" << RED << "Server terminated!" << RESET;
	}
	std::cout << std::endl;
}

int	main(int argc, char *argv[])
{
	if (argc != 3)
		return (1);

	Server*	serv = NULL;
	
	try
	{
		serv = new Server(argv[1], argv[2]);
		
		serverStatePrint(true);

		serv->run();
		serv->stop();
		
		serverStatePrint(false);
	}
	catch(const std::exception& e)
	{
		std::cerr << "main-catch:" << e.what() << std::endl;
		delete serv;
		serverStatePrint(false);

		return (1);
	}
	delete serv;

	return (0);
}
