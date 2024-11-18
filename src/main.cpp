/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:14:54 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/18 12:21:33 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_test_header.hpp"

int	main(void)
{
	// test_server();
	test_parser();
	// test_log_error();
	// test_send_error_response();
	// test_custom_err();

	return (0);
}

/*
Commands to Implement
Authentication Commands:

PASS: Client provides the connection password.
NICK: Client sets their nickname.
USER: Client sets their username.
Channel Management Commands:

JOIN: Join a specific channel.
PART: Leave a specific channel.
TOPIC: View or change the channel's topic.
MODE: Change channel modes (i, t, k, o, l).
KICK: Eject a client from a channel.
INVITE: Invite a client to a channel.
Messaging Commands:

PRIVMSG: Send a private message to a user or channel.
NOTICE: Send a notice to a user or channel.

*/
