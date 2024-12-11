/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namoisan <namoisan@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:14:54 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/11 10:43:49 by namoisan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_test_header.hpp"
#include "Server.hpp"
#include "ChannelTest.hpp"

int	main(int ac, char **av)
{
	// test_server();
	// test_parser();
	// test_log_error();
	// test_send_error_response();
	// test_custom_err();

	// if (ac != 3)
	// 	return 1;
	// Server serv(av[1], av[2]);
	// serv.start();
	(void)ac;
	(void)av;
	
	
	// test_parser();
	ChannelTest test;
	test.channelTests();
	// test_tokenizer(argc, argv);
	// test_reply();

	return (0);
}
