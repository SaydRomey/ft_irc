#include "Server.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <fcntl.h>
#include <utility>
#include <csignal> //SIGINT, SIGQUIT
#include <cerrno> // errno, EINTR

static const std::pair<std::string, int> cmdArr[CMD_UNKNOWN] = {
	std::make_pair("PASS", PASS),
	std::make_pair("NICK", NICK),
	std::make_pair("USER", USER),
	std::make_pair("JOIN", JOIN),
	std::make_pair("PART", PART),
	std::make_pair("TOPIC", TOPIC),
	std::make_pair("MODE", MODE),
	std::make_pair("KICK", KICK),
	std::make_pair("INVITE", INVITE),
	std::make_pair("PRIVMSG", PRIVMSG),
	std::make_pair("NOTICE", NOTICE),
	std::make_pair("PING", PING),
};
t_strIntMap Server::commandMap(cmdArr, cmdArr + CMD_UNKNOWN);

Server::Server(const std::string &port, const std::string &password): _time(time(NULL)),
	_clientMap(), _nickMap()
{
	// 
	_chanManager = new ChannelManager(*this); //
	// 
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

	pollfd pfd = {sockfd, POLLIN, 0};
	_pollFds.push_back(pfd);
}

Server::~Server() { delete _chanManager; }

void Server::run(void)
{
	char buffer[1024];
	ssize_t bytes;

	signal(SIGINT, Server::signalHandler); // catch the signal (CTRL + c)
	signal(SIGQUIT, Server::signalHandler); // catch the signal (CTRL + \)
	while (true)
	{
		for (t_pfdVect::iterator it=_pollFds.begin()+1; it != _pollFds.end(); it++)
		{
			if (_clientMap[it->fd].pendingSize() > 0)
				it->events |= POLLOUT;
		}
		if (!_safePoll())
			break;
		if (_pollFds[0].revents & POLLIN)
			_acceptConnection();
		for (t_pfdVect::iterator it=_pollFds.begin()+1; it != _pollFds.end(); it++)
		{
			User& client = _clientMap[it->fd];
			if (it->revents & POLLHUP)
			{
				it = _closeConnection(it) - 1;
				continue;
			}
			if (it->revents & POLLIN)
			{
				bzero(buffer, sizeof(buffer));
				bytes = recv(it->fd, buffer, sizeof(buffer), 0);
				if (bytes > 0)
					client.addToMsgBuffer(std::string(buffer, bytes));
				else
					client.resetMsgBuffer("QUIT :Obsolete message?\r\n");
			}
			if (it->revents & POLLOUT)
			{
				for (size_t n=client.pendingSize(); n > 0; n--)
				{
					std::string reply = client.pendingPop();
					send(it->fd, reply.c_str(), reply.size(), 0);
				}
				it->events ^= POLLOUT;
			}

			std::string msg_str = client.extractFromBuffer();
			while (!msg_str.empty())
			{
				// Message msg(msg_str);
				Message msg(msg_str, client.getNickname()); // *!!
				std::cout << msg << std::endl;
				// 
				if (msg.isValid() == true)
				// if (msg.getReply().empty())
					_messageRoundabout(client, msg);
				else if (msg.getCommand() != "NOTICE")
					client.pendingPush(msg.getReply());
				msg_str = client.extractFromBuffer();
			}
		}
	}
	// TODO: Look up how to close server socket
}

void Server::stop()
{
	for (t_pfdVect::iterator it=_pollFds.begin()+1; it != _pollFds.end(); it++)
	{
		send(it->fd, "ERROR :Server shutdown\r\n", 24, 0);
		close(it->fd);
	}
	close(_pollFds[0].fd);
}

void Server::signalHandler(int signum)
{
	(void)signum;
	std::cerr << RED << "\nInterrupt signal received!" << RESET << std::endl;
	// Server::_isRunning = false;
}

int Server::_safePoll()
{
	int ret = poll(&_pollFds[0], _pollFds.size(), -1);
	if (ret < 0 && errno != EINTR)
		throw std::runtime_error("Poll failed");
	return (ret >= 0);
}

void Server::_acceptConnection()
{
	// sockaddr_in	clientAddr;
	// socklen_t	clientLen = sizeof(clientAddr);

	int	clientFd = accept(_pollFds[0].fd, NULL, NULL);
	if (clientFd < 0)
		return ;

	pollfd pfd = {clientFd, POLLIN, 0};
	fcntl(clientFd, F_SETFL, O_NONBLOCK);
	_pollFds.push_back(pfd);
	_clientMap[clientFd] = User(clientFd);

	_pollFds[0].revents = 0;
	std::cout << "Client connected (fd: " << clientFd << ")." << std::endl;
}

t_pfdVect::iterator Server::_closeConnection(t_pfdVect::iterator &it)
{
	// TODO: remove User from all channels
	_nickMap.erase(_clientMap[it->fd].getNickname());
	_clientMap.erase(it->fd);
	close(it->fd);
	return _pollFds.erase(it);
}

void Server::_messageRoundabout(User& client, const Message& msg)
{
	switch (Server::commandMap[msg.getCommand()])
	{
	case PASS:
		pass_cmd(client, msg);
		break;
	case USER:
		user_cmd(client, msg);
		break;
	case NICK:
		nick_cmd(client, msg);
		break;
	case JOIN:
		_chanManager.join(client, msg);
		break;
	case PART:
		_chanManager.part(client, msg);
		break;
	case TOPIC:
		_chanManager.topic(client, msg);
		break;
	case MODE:
		_chanManager.mode(client, msg);
		break;
	case KICK:
		_chanManager.kick(client, msg);
		break;
	case INVITE:
		break;
	case PRIVMSG:
		privmsg_cmd(client, msg);
		break;
	case NOTICE:
		break;
	case PING:
		break;
	default:
		break;
	}
}

void Server::broadcast(const std::string &msg, int senderFd)
{
	for (t_clientMap::iterator it=_clientMap.begin(); it != _clientMap.end(); it++)
	{
		if (it->first == senderFd)
			it->second.pendingPush(msg);
	}
}

void Server::pass_cmd(User &client, const Message& msg)
{
	std::cout << "getParams() => <" << msg.getParams() << ">" << std::endl;
	short perms = client.getPerms();
	if (perms == PERM_ALL)
		client.pendingPush(reply(462, client.getNickname()));
	else
		client.setPass(msg.getParams() == _password);
}

void Server::user_cmd(User &client, const Message& msg)
{
	short perms = client.getPerms();
	if (perms == PERM_ALL)
		client.pendingPush(reply(462, client.getNickname()));
	else if (perms == PERM_NICK)
		client.pendingPush(reply(464, client.getNickname()));
	else
	{
		client.setUsername(msg.getParams());
		if (client.getPerms() == PERM_ALL)
			client.pendingPush(reply(1, client.getNickname(), client.getNickname()));
	}
}

void Server::nick_cmd(User &client, const Message& msg)
{
	static const std::string leadCharBan = "#&:0123456789";

	short perms = client.getPerms();
	if (perms == PERM_USER)
	{
		client.pendingPush(reply(464, client.getNickname()));
		return;
	}

	const std::string& nick = msg.getParams();
	std::string oldNick = client.getNickname();
	if (_nickMap.count(nick) == 1)
		client.pendingPush(reply(433, oldNick, nick));
	else if (leadCharBan.find(nick[0]) != std::string::npos
			|| nick.find(' ') != std::string::npos)
		client.pendingPush(reply(432, oldNick, nick));
	else
	{
		_nickMap.erase(oldNick);
		_nickMap[nick] = client.getFd();
		client.setNickname(nick);
		std::string msg = ":" + oldNick + "!" + client.getUsername() + "@ft-irc NICK " + nick;
		broadcast(msg);
		if (perms == ~PERM_NICK)
			client.pendingPush(reply(1, nick, nick));
	}
}

void Server::privmsg_cmd(User &client, const Message &msg)
{
	(void)client;
	(void)msg;
	// std::vector<std::string> targets(tokenize(msg.getParams(), ','));

	// for (size_t i=0; i < targets.size(); i++)
	// {
	// 	if (targets[i][0] == '#')
	// 		_chanManager.privmsg(client, targets[i], msg.getReply());
	// 	else if (_nickMap.count(targets[i]) == 0)
	// 		client.pendingPush(reply(401, targets[i]));
	// 	else if (_nickMap[targets[i]] != client.getFd())
	// 		_clientMap[_nickMap[targets[i]]].pendingPush(msg.getReply());
	// }
}

// 
User*	Server::getUserByNickname(const std::string& nickname)
{
	// Check if the nickname exists in the _nickMap
	t_strIntMap::iterator	it = _nickMap.find(nickname);
	if (it != _nickMap.end())
	{
		int	fd = it->second; // Get the corresponding FD
		t_clientMap::iterator	clientIt = _clientMap.find(fd);
		if (clientIt != _clientMap.end())
		{
			return &clientIt->second; // Return the User object
		}
	}
	return NULL;
}
// 
