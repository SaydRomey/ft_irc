
#include "_test_header.hpp"
#include "Server.hpp"
#include "Message.hpp"

int	test_server(int ac, char **av)
{
	Server* serv = NULL;
	if (ac != 3)
		return 1;
	try
	{
		serv = new Server(av[1], av[2]);
		serv->run();
		serv->stop();
	}
	catch(const std::exception& e)
	{
		std::cerr << "main-catch:" << e.what() << std::endl;
		delete serv;
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
