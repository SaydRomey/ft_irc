
#include "_test_header.hpp"
// #include "Server.hpp"

// int	test_server(int ac, char **av)
// {
// 	if (ac != 3)
// 	{
// 		return (1);
// 	}
// 	ft::Server	serv(av[1], av[2]);
// 	serv.start();

// 	return (0);
// }

#include "Message.hpp"

void	tmp_test(int argc, char *argv[])
{
	if (argc != 2)
		return ;

	Message msg(argv[1]);
	std::cout << msg << std::endl;
}

int	main(int argc, char *argv[])
{
	// test_server(argc, argv);
	// test_channel();

	tmp_test(argc, argv);	
	test_message();
	test_reply();
	test_pseudo_replies();
	test_reply_templates();
	test_welcome_replies();
	
	// test_tokenizer(argc, argv);

	return (0);
}
