#include "Server.hpp"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <fcntl.h>

Server::Server(const std::string &port, const std::string &password)
{
	long tmp_port = std::strtol(port.c_str(), NULL, 10);
	if (tmp_port < 1024 || tmp_port > 65535 || port.find_first_not_of("0123456789") != std::string::npos)
		throw std::invalid_argument("Server::invalid_port");
	_port = tmp_port;

	if (password == "" || password.find_first_of("\t\n\r\f\v ") != std::string::npos)
		throw std::invalid_argument("Server::invalid_password");
	_password = password;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		throw std::runtime_error("Server::socket_creation_failed");
	fcntl(sockfd, F_SETFL, O_NONBLOCK);

	sockaddr_in serverAddr;
	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(_port);
	if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
		throw std::runtime_error("Server::socket_bind_failed");

	if (listen(sockfd, SOMAXCONN) < 0)
		throw std::runtime_error("Server::socket_listen_failed");

	int	opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("Server::setsockopt_failed");

	_pollFds.push_back({sockfd, POLLIN, 0});
	time(&_time);
	_isRunning = false;
}

void Server::run(void)
{
	char buffer[1024];
	ssize_t bytes;

	_isRunning = true;
	while (_isRunning)
	{
		for (t_pollfdVect::iterator it=_pollFds.begin()+1; it != _pollFds.end(); it++)
		{
			if (_clients[it->fd].pending.size() > 0)
				it->events |= POLLOUT;
		}
		if (poll(&_pollFds[0], _pollFds.size(), -1) < 0)
			throw std::runtime_error("Poll failed");
		if (_pollFds[0].revents & POLLIN)
			_acceptConnection();
		for (t_pollfdVect::iterator it=_pollFds.begin()+1; it != _pollFds.end(); it++)
		{
			if (it->revents & POLLHUP)
			{
				// TODO
			}
			if (it->revents & POLLIN)
			{
				bzero(buffer, sizeof(buffer));
				bytes = recv(it->fd, buffer, sizeof(buffer), 0);
				if (bytes > 0)
					_clients[it->fd].addToMsgBuffer(std::string(buffer, bytes));
				else
					_clients[it->fd].resetMsgBuffer("QUIT :Connection reset by peer\r\n");
			}
			if (it->revents & POLLOUT)
			{
				while (_clients[it->fd].pending.size() > 0)
				{
					std::string msg = _clients[it->fd].pending.front();
					send(it->fd, msg.c_str(), msg.size(), 0);
					_clients[it->fd].pending.pop();
				}
				it->events ^= POLLOUT;
			}
			// TODO: handle other revents flags maybe
		}
		for (t_clientMap::iterator it=_clients.begin(); it != _clients.end(); it++)
		{
			std::string msg_str = it->second.extractFromBuffer();
			while (msg_str != "")
			{
				try
				{
					Message msg(msg_str);
					_messageRoundabout(msg);
				} catch (std::invalid_argument& e)
				{
					std::cout << "Invalid argument: " << e.what() << std::endl;
				}
				msg_str = it->second.extractFromBuffer();
			}
		}
	}
}

void Server::_acceptConnection()
{
	// sockaddr_in	clientAddr;
	// socklen_t	clientLen = sizeof(clientAddr);

	int	clientFd = accept(_pollFds[0].fd, NULL, NULL);
	if (clientFd < 0)
		return ;

	fcntl(clientFd, F_SETFL, O_NONBLOCK);
	_pollFds.push_back({clientFd, POLLIN, 0});
	_clients[clientFd] = ft::Client(clientFd);

	_pollFds[0].revents = 0;
	std::cout << "Client connected (fd: " << clientFd << ")." << std::endl;
}

void Server::_messageRoundabout(const Message& msg)
{
	switch (Validator::commandMap[msg.getCommand()])
	{
	case PASS:
		// TODO
		break;
	case NICK:
		break;
	case USER:
		break;
	case JOIN:
		break;
	case PART:
		break;
	case TOPIC:
		break;
	case MODE:
		break;
	case KICK:
		break;
	case INVITE:
		break;
	case PRIVMSG:
		break;
	case NOTICE:
		break;
	default:
		break;
	}
}