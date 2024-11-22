/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:47:25 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/22 17:16:12 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9

#include "test_server.hpp"

/*
compile and launch the executable,
then in another terminal, enter
	$> nc 127.0.0.1 4444

messages written in that terminal should print in the one where this code was compiled
*/

Client::Client()
{
	// 
}

int	Client::getFd()
{
	return (_fd);
}

void	Client::setFd(int fd)
{
	_fd = fd;
}

void	Client::setIpAdd(std::string ipadd)
{
	_ipAdd = ipadd;
}

/* ************************************************************************** */

Server::Server()
{
	_serSocketFd = -1;
}

void	Server::ClearClients(int fd)
{
	size_t	i = 0;
	// remove the client from the pollfd
	while (i < _fds.size())
	{
		if (_fds[i].fd == fd)
		{
			_fds.erase(_fds.begin() + i);
			break ;
		}
		i++;
	}
	// remove the clients from the vector of clients
	i = 0;
	while (i < _clients.size())
	{
		if (_clients[i].getFd() == fd)
		{
			_clients.erase(_clients.begin() + i);
			break ;
		}
		i++;
	}
}

bool	Server::_signal = false; // initialize the static boolean

void	Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << "\n" << "Signal Recieved!" << std::endl;
	Server::_signal = true; // set the static boolean to true to stop the server
}

void	Server::CloseFds()
{
	size_t	i = 0;
	// close all the clients
	while (i < _clients.size())
	{
		std::cout << RED << "Client <" << _clients[i].getFd() << "> Disconnected" << RESET << std::endl;
		close(_clients[i].getFd());
		i++;
	}
	if (_serSocketFd != -1)
	{
		std::cout << RED << "Server <" << _serSocketFd << "> Disconnected" << RESET << std::endl;
		close(_serSocketFd);
	}
}

void	Server::SerSocket()
{
	struct sockaddr_in	add;
	struct pollfd		NewPoll;

	add.sin_family = AF_INET; // set the address family to ipv4
	add.sin_port = htons(this->_port); // convert the port to network byte order (big endian)
	// add.sin_addr.s_addr = INADDR_ANY; // set the address to any local machine address
	add.sin_addr.s_addr = inet_addr("127.0.0.1");

	std::cout << "Creating socket..." << std::endl;
	_serSocketFd = socket(AF_INET, SOCK_STREAM, 0); // create the server socket
	if (_serSocketFd == -1) // check if the socket is created
		throw (std::runtime_error("Failed to create socket"));
	
	std::cout << "Setting socket options..." << std::endl;
	int	en = 1;
	if (setsockopt(_serSocketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1) // set the socket option (SO_REUSEADDR) to reuse the address
		throw (std::runtime_error("Failed to set option (SO_REUSEADDR) on socket"));
		
	if (fcntl(_serSocketFd, F_SETFL, O_NONBLOCK) == -1) // set the socket option (O_NONBLOCK) for non-blocking socket
		throw (std::runtime_error("Failed to set option (O_NONBLOCK) on socket"));
		
	if (bind(_serSocketFd, (struct sockaddr *)&add, sizeof(add)) == -1) // bin the socket to the address
		throw (std::runtime_error("Failed to bind socket"));
		
	if (listen(_serSocketFd, SOMAXCONN) == -1) // listen for incomming connections and makes the socket a passive socket
		throw (std::runtime_error("listen() failed"));
	
	std::cout << "Server socket initialized on port " << this->_port << std::endl;
	std::cout << "Server bound to: 127.0.0.1:" << _port << std::endl;
	
	NewPoll.fd = _serSocketFd; // add the server socket to the pollfd
	NewPoll.events = POLLIN; // set the event to POLLIN for reading data
	NewPoll.revents = 0; // set the revents to 0
	_fds.push_back(NewPoll); // add the server socket to the pollfd
}

void	Server::ServerInit()
{
	this->_port = 6667;
	SerSocket(); // create the server socket

	std::cout << GREEN << "Server <" << _serSocketFd << "> Connected" << RESET << std::endl;
	std::cout << "Waiting to accept a connection..." << std::endl;

	while (Server::_signal == false) // run the server until the signal is recieved
	{
		if ((poll(&_fds[0], _fds.size(), -1) == -1) && Server::_signal == false) // wait for an event
			throw (std::runtime_error("poll() failed"));
		
		size_t	i = 0;
		while (i < _fds.size())
		{
			// check all file descriptors
			if (_fds[i].revents & POLLIN) // check if there is data to read
			{
				if (_fds[i].fd == _serSocketFd)
					AcceptNewClient(); // accept new client
				else
					RecieveNewData(_fds[i].fd); // recieve new data from a registered client
			}
			i++;
		}
	}
	CloseFds(); // close the file descriptor when the server stops
}

void	Server::AcceptNewClient()
{
	Client	client;
	struct sockaddr_in	cliadd;
	struct pollfd		NewPoll;
	socklen_t			len = sizeof (cliadd);

	int	incofd = accept(_serSocketFd, (sockaddr *)&(cliadd), &len); // accept the new client
	if (incofd == -1)
	{
		std::cout << "accept() failed" << std::endl;
		return ;
	}
	
	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1) // set the socket option (O_NONBLOCK) for non->blocking socket
	{
		std::cout << "fcntl() failed" << std::endl;
		return ;
	}
	
	NewPoll.fd = incofd; // add the client socket to the pollfd
	NewPoll.events = POLLIN; // set the events to POLLIN for reading data
	NewPoll.revents = 0; // set the revents to 0

	client.setFd(incofd); // set the client file descriptor
	client.setIpAdd(inet_ntoa((cliadd.sin_addr))); // convert the ip address to string and set it
	_clients.push_back(client); // add the client to the vector of clients
	_fds.push_back(NewPoll); // add the client socket to the pollfd

	std::cout << GREEN << "Client <" << incofd << "> Connected" << RESET << std::endl;
}

void	Server::RecieveNewData(int fd)
{
	char	buff[1024]; // buffer for the recieved data
	memset(buff, 0, sizeof(buff)); // clear the buffer

	ssize_t	bytes = recv(fd, buff, sizeof(buff) - 1, 0); // recieve the data

	if (bytes <= 0) // check if the client disconnected
	{
		std::cout << RED << "Client <" << fd << "> Disconnected" << RESET << std::endl;
		ClearClients(fd); // clear the client
		close(fd); // close the client socket
	}
	else
	{
		// print the recieved data
		buff[bytes] = '\0';
		std::string	data(buff);

		// here we add code to process the recueved data: parse, check, authenticate, handle the commands, etc...
		
		// tmp examples for testing
		
		if (data.find("NICK") == 0)
		{
			// Respond to NICK
			std::string	nick = data.substr(5);
			std::string	reply = ":ft_irc 001 " + nick + " :Welcome to the server\r\n";
			send(fd, reply.c_str(), reply.size(), 0);
		}
		else if (data.find("USER") == 0)
		{
			// Respond to USER
			std::string	reply = ":ft_irc 001 user :User registered\r\n";
			send(fd, reply.c_str(), reply.size(), 0);
		}
		else if (data.find("PING") == 0)
		{
			// Respond to PING with PONG
			std::string	pong = "PONG :ft_irc\r\n";
			send(fd, pong.c_str(), pong.size(), 0);
		}
		else
		{
			// handle generic messages
			std::cout << YELLOW << "Client <" << fd << "> Data: " RESET << buff;
		}
	}
}

void	test_server(void)
{
	Server	server;
	
	std::cout << "---- SERVER ----" << std::endl;
	try
	{
		signal(SIGINT, Server::SignalHandler); // catch the signal (CTRL + c)
		signal(SIGQUIT, Server::SignalHandler); // catch the signal (CTRL + \)
		server.ServerInit(); // initialize the server
	}
	catch (std::exception &e)
	{
		server.CloseFds(); // close the file descriptors
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The server closed!" << std::endl;
}
