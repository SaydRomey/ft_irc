
#include "_OldServer.hpp"
#include <iostream>
#include <cstring> // for strerror
#include <cstdlib>
#include <unistd.h> // for close()
#include <ctime>
#include <fcntl.h>

ft::Server::Server(const std::string &port, const std::string &password) \
: _port(port), _password(password), _isRunning(false), _serverFd(-1)
{
	// 
}

ft::Server::~Server(void)
{
	stop();
}

void	ft::Server::_initSocket(void)
{
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd < 0)
		throw (std::runtime_error(std::string("Socket creation failed: ").append(strerror(errno))));
	fcntl(_serverFd, F_SETFL, O_NONBLOCK);

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

void	ft::Server::start(void)
{
	_isRunning = true;
	_initSocket();
	time(&_startTime);

	pollfd	serverPollFd = {_serverFd, POLLIN, 0};
	_pollFds.push_back(serverPollFd);

	while (_isRunning)
	{
		std::cout << PURPLE << "[DEBUG] Polling... " << _pollFds.size() << RESET << std::endl;
		if (poll(&_pollFds[0], _pollFds.size(), -1) < 0)
			throw (std::runtime_error("Poll failed: " + std::string(strerror(errno))));
		std::cout << PURPLE << "[DEBUG] Polling complete! " << _pollFds.size() << RESET << std::endl;

		size_t	i = 0;
		while (i < _pollFds.size())
		{
			if (_pollFds[i].fd == _serverFd)
				_acceptConnection();
			else if (_pollFds[i].revents & POLLIN)
				_handleClient(_pollFds[i].fd);
			++i;
		}
	}
}

void	ft::Server::stop(void)
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


void	ft::Server::_acceptConnection(void)
{
	sockaddr_in	clientAddr;
	socklen_t	clientLen = sizeof(clientAddr);

	int	clientFd = accept(_serverFd, (struct sockaddr *)&clientAddr, &clientLen);
	fcntl(clientFd, F_SETFL, O_NONBLOCK);

	if (clientFd < 0)
		return ;

	pollfd	clientPollFd = {clientFd, POLLIN, 0};
	_pollFds.push_back(clientPollFd);
	_clients[clientFd] = ft::Client(clientFd);

	std::cout << "Client connected (fd: " << clientFd << ")." << std::endl;
	// _handleClient(clientFd);
}

void	ft::Server::_handleClient(int clientFd)
{
	char	buffer[1024];
	int	bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

	if (bytesRead < 0)
	{
		std::cout << "   HERE?" << std::endl;
		_disconnectClient(clientFd, "Disconnected");
		return ;
	}

	buffer[bytesRead] = '\0';
	std::string	data(buffer);
	std::cout << YELLOW << "Client <" << clientFd << "> Data: " RESET << data << std::flush;

	size_t start = 0;
	size_t end = data.find("\r\n", 0);
	std::string command;
	while (end != std::string::npos)
	{
		command = data.substr(start, end - start);
		start = end + 2;
		end = data.find("\r\n", start);
		if (command == "CAP LS 302")
			continue;
		try
		{
			Message	message = _parser.parse(command);
			std::cout << "    Command: " << message.str();
			_handleCommand(clientFd, message);
		}
		catch (const std::exception& e)
		{
			_clients[clientFd].sendMessage("ERROR " + std::string(e.what()));
			_disconnectClient(clientFd, e.what());
			break;
		}
	}
	std::cout << PURPLE << "END OF _handleClient loop" << RESET << std::endl;
	
	// std::vector<std::string>	commands = _aggregator.processData(clientFd, data);
	// size_t	i = 0;
	// while (i < commands.size())
	// {
	// 	std::cout << YELLOW << "Client <" << clientFd << "> Data: " RESET << commands[i] << std::endl;
	// 	try
	// 	{
	// 		Message	message = _parser.parse(commands[i]);
	// 		_handleCommand(clientFd, message);
	// 	}
	// 	catch (const std::exception &e)
	// 	{
	// 		_clients[clientFd].sendMessage("ERROR " + std::string(e.what()));
	// 		_disconnectClient(clientFd, e.what());
	// 		break;
	// 	}
	// 	++i;
	// }
}

void	ft::Server::_broadcast(const std::string &message, int senderFd)
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

void ft::Server::_welcomeClient(int clientFd)
{
	Client&	client = _clients[clientFd];
	Message	msg;

	msg.setPrefix(":ft-irc");
	msg.setCommand("001");
	msg.setParams(client.getNickname());
	msg.setTrailing("Placeholder text");
	client.sendMessage(msg.str());

	msg.setCommand("002");
	client.sendMessage(msg.str());

	msg.setCommand("003");
	client.sendMessage(msg.str());

	msg.setCommand("004");
	client.sendMessage(msg.str());
}

void	ft::Server::_handleCommand(int clientFd, const Message &message)
{
	const std::string	&command = message.getCommand();
	ft::Client&			client = _clients[clientFd];

	if (command == "PASS")
	{
		if (client.isAuthenticated())
			return;
		_authenticateClient(clientFd, message);
	}
	else if (!client.isAuthenticated())
	{
		client.sendMessage(":ft-irc 464 :Password is wrong/not supplied");
		throw std::logic_error("Password error");
	}
	else if (command == "NICK")
		client.setNickname(message.getParams());
	else if (command == "USER")
	{
		const std::string& params = message.getParams();
		client.setUsername(params.substr(0, params.find(' ')));
	}
	else if (command == "PING")
	{
		_sendMessage(clientFd, "PONG :" + message.getTrailing());
	}
	else if (command == "PRIVMSG")
	{
		_broadcast(message.getTrailing(), clientFd);
	}
	// else
	// {
	// 	_sendMessage(clientFd, "ERROR: Unknown command");
	// }
}

void	ft::Server::_authenticateClient(int clientFd, const Message &message)
{
	if (message.getParams() == _password)
	{
		_clients[clientFd].setAuthenticated(true);
		std::cout << "Client authenticated (fd: " << clientFd << ")." << std::endl;
	}
	else
	{
		_disconnectClient(clientFd, "Authentication failed.");
	}
}

void	ft::Server::_sendMessage(int clientFd, const std::string &message)
{
	std::string	formattedMessage = message + "\r\n";
	send(clientFd, formattedMessage.c_str(), formattedMessage.size(), 0);
}

void	ft::Server::_disconnectClient(int clientFd, const std::string &reason)
{
	std::cout << "Disconnecting client (fd: " << clientFd << "): " << reason << std::endl;
	_clients.erase(clientFd);
	close(clientFd);

	for (std::vector<pollfd>::iterator it=_pollFds.begin(); it != _pollFds.end(); it++)
	{
		if (it->fd != clientFd)
			continue;
		_pollFds.erase(it);
		break;
	}
}
