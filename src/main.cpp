/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdemers <jdemers@student.42quebec.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:14:54 by cdumais           #+#    #+#             */
/*   Updated: 2025/01/10 20:30:25 by jdemers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_test_header.hpp"
#include "Server.hpp"

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

int	main(int ac, char **av)
{
	int val = 0;

	val = test_server(ac, av);
	// test_parser();
	// ChannelTest test;
	// test.channelTests();
	// test_tokenizer(argc, argv);
	// test_reply();

	return (val);
}
