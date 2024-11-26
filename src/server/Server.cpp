
#include "Server.hpp"
#include <iostream>
#include <cstring> // for strerror
#include <cstdlib>
#include <unistd.h> // for close()

Server::Server(const std::string &port, const std::string &password) \
: _port(port), _password(password), _isRunning(false), _serverFd(-1)
{
	// 
}

Server::~Server(void)
{
	stop();
}

void	Server::_initSocket(void)
{
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd < 0)
		throw (std::runtime_error(std::string("Socket creation failed: ").append(strerror(errno))));

	int	opt = 1;
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw (std::runtime_error(std::string("setsockopt failed: ").append(strerror(errno))));

	sockaddr_in	serverAddr;
	std::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(atoi(_port.c_str()));

	if (bind(_serverFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
		throw (std::runtime_error("Binding failed: " + std::string(strerror(errno))));

	if (listen(_serverFd, SOMAXCONN) < 0)
		throw (std::runtime_error("Listen failed: " + std::string(strerror(errno))));

	std::cout << "Server initialized and listening on port " << _port << std::endl;
}

void	Server::start(void)
{
	_isRunning = true;
	_initSocket();

	pollfd	serverPollFd = {_serverFd, POLLIN, 0};
	_pollFds.push_back(serverPollFd);

	while (_isRunning)
	{
		if (poll(&_pollFds[0], _pollFds.size(), -1) < 0)
			throw (std::runtime_error("Poll failed: " + std::string(strerror(errno))));

		size_t	i = 0;
		while (i < _pollFds.size())
		{
			if (_pollFds[i].fd == _serverFd)
				_acceptConnection();
			else
				_handleClient(_pollFds[i].fd);
			++i;
		}
	}
}

void	Server::stop(void)
{
	_isRunning = false;

	size_t	i = 0;
	while (i < _pollFds.size())
	{
		close(_pollFds[i].fd);
		++i;
	}
	if (_serverFd >= 0)
		close(_serverFd);

	std::cout << "Server stropped." << std::endl;
}


void	Server::_acceptConnection(void)
{
	sockaddr_in	clientAddr;
	socklen_t	clientLen = sizeof(clientAddr);

	int	clientFd = accept(_serverFd, (struct sockaddr *)&clientAddr, &clientLen);

	if (clientFd < 0)
	{
		std::cout << "Failed to accept connection: " << strerror(errno) << std::endl;
		return ;
	}

	pollfd	clientPollFd = {clientFd, POLLIN, 0};
	_pollFds.push_back(clientPollFd);
	_clients[clientFd] = ft::Client(clientFd);

	std::cout << "Client connected (fd: " << clientFd << ")." << std::endl;
}

void	Server::_handleClient(int clientFd)
{
	char	buffer[1024];
	int	bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

	if (bytesRead <= 0)
	{
		_disconnectClient(clientFd, "Disconnected");
		return ;
	}

	buffer[bytesRead] = '\0';
	std::string	data(buffer);

	std::vector<std::string>	commands = _aggregator.processData(clientFd, data);

	size_t	i = 0;
	while (i < commands.size())
	{
		try
		{
			Message	message = _parser.parse(commands[i]);
			_handleCommand(clientFd, message);
		}
		catch (const std::exception &e)
		{
			std::cout << "Error handling command: " << e.what() << std::endl;
			_sendMessage(clientFd, "ERROR: " + std::string(e.what()));
		}
		++i;
	}
}

void	Server::_broadcast(const std::string &message, int senderFd)
{
	size_t	i = 0;

	while (i < _pollFds.size())
	{
		int	clientFd = _pollFds[i].fd;
		if (clientFd != _serverFd && clientFd != senderFd)
			_sendMessage(clientFd, message);
		++i;
	}
}

void Server::_welcomeClient(int clientFd)
{
	Client& client = _clients[clientFd];
	std::string msg;

	msg = "001 "
}

void	Server::_handleCommand(int clientFd, const Message &message)
{
	const std::string	&command = message.getCommand();

	if (command == "PASS")
	{
		_authenticateClient(clientFd, message);
	}
	else if (command == "PING")
	{
		_sendMessage(clientFd, "PONG :" + message.getTrailing());
	}
	else if (command == "PRIVMSG")
	{
		_broadcast(message.getTrailing(), clientFd);
	}
	else
	{
		_sendMessage(clientFd, "ERROR: Unknown command");
	}
}

void	Server::_authenticateClient(int clientFd, const Message &message)
{
	if (message.getTrailing() == _password)
	{
		_sendMessage(clientFd, "Password accepted.");
		std::cout << "Client autheticated (fd: " << clientFd << ")." << std::endl;
	}
	else
	{
		_disconnectClient(clientFd, "Authentication failed.");
	}
}

void	Server::_sendMessage(int clientFd, const std::string &message)
{
	std::string	formattedMessage = message + "\r\n";
	send(clientFd, formattedMessage.c_str(), formattedMessage.size(), 0);
}

void	Server::_disconnectClient(int clientFd, const std::string &reason)
{
	std::cout << "Disconnecting client (fd: " << clientFd << "): " << reason << std::endl;
	_clients.erase(clientFd);
	close(clientFd);

	// const int AAA = clientFd;
	// _pollFds.erase(std::remove_if(_pollFds.begin(), _pollFds.end(), _is_clientFd<AAA>), _pollFds.end());
	// _clients.erase(clientFd);
}
