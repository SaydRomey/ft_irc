
#include "_test_header.hpp"
#include "Server.hpp"
#include "Message.hpp"

/*
/server add ircserv localhost/6667 -notls -password=your_password

/set irc.server.ircserv.capabilities ""

*/

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

int	test_server(int ac, char **av)
{
	Server* serv = NULL;
	if (ac != 3)
		return 1;
	try
	{
		serv = new Server(av[1], av[2]);
		
		// 
		serverStatePrint(true);
		// 

		serv->run();
		serv->stop();
		
		// 
		serverStatePrint(false);
		// 
	}
	catch(const std::exception& e)
	{
		std::cerr << "main-catch:" << e.what() << std::endl;
		delete serv;
		// 
		serverStatePrint(false);
		// 
		return 1;
	}
	delete serv;
	return (0);
}

int	main(int argc, char *argv[])
{
	test_server(argc, argv);
	
	// test_channel();

	// test_message();
	// test_arg_message(argc, argv);

	// test_reply();
	// test_pseudo_replies();
	// test_reply_templates();
	// test_welcome_replies();
	
	// test_tokenizer(argc, argv);

	return (0);
}
