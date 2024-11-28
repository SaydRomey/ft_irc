/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdemers <jdemers@student.42quebec.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:14:54 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/18 17:58:39 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_test_header.hpp"
#include "Server.hpp"

int	main(int ac, char **av)
{
	// test_server();
	// test_parser();
	// test_log_error();
	// test_send_error_response();
	// test_custom_err();

	if (ac != 3)
		return 1;
	ft::Server serv(av[1], av[2]);
	serv.start();

	return (0);
}
