/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:14:54 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/20 14:53:58 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	// test_tokenizer(argc, argv);
	// test_reply();

	return (0);
}
