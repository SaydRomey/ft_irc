// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   test_server.hpp                                    :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/11/21 15:37:24 by cdumais           #+#    #+#             */
// /*   Updated: 2024/11/22 17:37:23 by cdumais          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include <iostream>
// #include <vector>		// vector
// #include <sys/socket.h>	// socket()
// #include <sys/types.h>	// socket()
// #include <netinet/in.h>	// sockaddr_in
// #include <fcntl.h>		// fcntl()
// #include <unistd.h>		// close()
// #include <arpa/inet.h>	// inet_ntoa()
// #include <poll.h>		// poll()
// #include <csignal>		// signal()
// #include <cstring>		// memset()

// /*
// compile and launch the executable,
// then in another terminal, enter
// 	$> nc 127.0.0.1 4444

// messages written in that terminal should print in the one where this code was compiled
// */

// # define RESET		"\033[0m"
// # define BOLD		"\033[1m"
// # define ITALIC		"\033[3m"
// # define UNDERLINE	"\033[4m"
// # define RED		"\033[31m"
// # define GREEN		"\033[32m"
// # define YELLOW		"\033[33m"
// # define BLUE		"\033[34m"
// # define PURPLE		"\033[35m"
// # define CYAN		"\033[36m"
// # define ORANGE		"\033[38;5;208m"
// # define GRAYTALIC	"\033[3;90m"

// /*
// Contains all the info about a specific client inside the server
// */
// class Client
// {
// 	private:
// 		int			_fd; // client file descriptor
// 		std::string	_ipAdd; // client ip address
	
// 	public:
// 		Client();
// 		int		getFd();

// 		void	setFd(int fd);
// 		void	setIpAdd(std::string ipadd);
// };

// /*
// Contains all the info about the server
// has a vector of clients to keep track of all the clients
// and manage their requests
// */
// class TestServer
// {
// 	private:
// 		int							_port; // server port
// 		int							_serSocketFd; // server socket file descriptor
// 		static bool					_signal; // static boolean for signal
// 		std::vector<Client>			_clients; // vector of clients
// 		std::vector<struct pollfd>	_fds; // vector of pollfd
	
// 	public:
// 		TestServer();
		
// 		void	TestServerInit(); // server initialization
// 		void	SerSocket(); // server socket creation
// 		void	AcceptNewClient(); // accept new client
// 		void	RecieveNewData(int fd); // recieve new data from a registered client

// 		static void	SignalHandler(int signum); // signal handler

// 		void	CloseFds(); // close file descriptors
// 		void	ClearClients(int fd); // clear clients
// };
