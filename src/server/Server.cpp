// #include "Server.hpp"
// #include <iostream>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <cstring>
// #include <fcntl.h>

// Server::Server(const std::string &port, const std::string &password): _time(time(NULL)),
// 	_isRunning(false), _clients(), _nickMap(), _chanManager(), _rplGenerator()
// {
// 	long tmp_port = std::strtol(port.c_str(), NULL, 10);
// 	if (tmp_port < 1024 || tmp_port > 65535 || port.find_first_not_of("0123456789") != std::string::npos)
// 		throw std::invalid_argument("Server::invalid_port");
// 	_port = tmp_port;

// 	if (password == "" || password.find_first_of("\t\n\r\f\v ") != std::string::npos)
// 		throw std::invalid_argument("Server::invalid_password");
// 	_password = password;

// 	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
// 	if (sockfd < 0)
// 		throw std::runtime_error("Server::socket_creation_failed");
// 	fcntl(sockfd, F_SETFL, O_NONBLOCK);

// 	sockaddr_in serverAddr;
// 	bzero(&serverAddr, sizeof(serverAddr));
// 	serverAddr.sin_family = AF_INET;
// 	serverAddr.sin_addr.s_addr = INADDR_ANY;
// 	serverAddr.sin_port = htons(_port);
// 	if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
// 		throw std::runtime_error("Server::socket_bind_failed");

// 	if (listen(sockfd, SOMAXCONN) < 0)
// 		throw std::runtime_error("Server::socket_listen_failed");

// 	int	opt = 1;
// 	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
// 		throw std::runtime_error("Server::setsockopt_failed");

// 	pollfd pfd = {sockfd, POLLIN, 0};
// 	_pollFds.push_back(pfd);
// 	_initRoundabout();
// }

// void Server::run(void)
// {
// 	char buffer[1024];
// 	ssize_t bytes;

// 	_isRunning = true;
// 	while (_isRunning)
// 	{
// 		for (t_pollfdVect::iterator it=_pollFds.begin()+1; it != _pollFds.end(); it++)
// 		{
// 			if (_clients[it->fd].pendingSize() > 0)
// 				it->events |= POLLOUT;
// 		}
// 		if (poll(&_pollFds[0], _pollFds.size(), -1) < 0)
// 			throw std::runtime_error("Poll failed");
// 		if (_pollFds[0].revents & POLLIN)
// 			_acceptConnection();
// 		for (t_pollfdVect::iterator it=_pollFds.begin()+1; it != _pollFds.end(); it++)
// 		{
// 			if (it->revents & POLLHUP)
// 			{
// 				// TODO
// 			}
// 			if (it->revents & POLLIN)
// 			{
// 				bzero(buffer, sizeof(buffer));
// 				bytes = recv(it->fd, buffer, sizeof(buffer), 0);
// 				if (bytes > 0)
// 					_clients[it->fd].addToMsgBuffer(std::string(buffer, bytes));
// 				else
// 					_clients[it->fd].resetMsgBuffer("QUIT :Connection reset by peer\r\n");
// 			}
// 			if (it->revents & POLLOUT)
// 			{
// 				for (size_t n=_clients[it->fd].pendingSize(); n > 0; n--)
// 				{
// 					std::string msg = _clients[it->fd].pendingPop();
// 					send(it->fd, msg.c_str(), msg.size(), 0);
// 				}
// 				it->events ^= POLLOUT;
// 			}
// 			// TODO: handle other revents flags maybe
// 		}
// 		for (t_clientMap::iterator it=_clients.begin(); it != _clients.end(); it++)
// 		{
// 			std::string msg_str = it->second.extractFromBuffer();
// 			while (!msg_str.empty())
// 			{
// 				Message msg(msg_str);
// 				std::cout << msg << std::endl;
// 				if (msg.getReply().empty())
// 					_messageRoundabout(it->second, msg);
// 				else if (msg.getCommand() != "NOTICE")
// 					it->second.pendingPush(msg.getReply());
// 				msg_str = it->second.extractFromBuffer();
// 			}
// 		}
// 	}
// }

// void Server::_initRoundabout()
// {
// 	_serverRoundabout["PASS"] = &Server::pass_cmd;
// 	_serverRoundabout["USER"] = &Server::user_cmd;
// 	_serverRoundabout["NICK"] = &Server::nick_cmd;
// 	_serverRoundabout["PRIVMSG"] = &Server::privmsg_cmd;
// }

// void Server::_acceptConnection()
// {
// 	// sockaddr_in	clientAddr;
// 	// socklen_t	clientLen = sizeof(clientAddr);

// 	int	clientFd = accept(_pollFds[0].fd, NULL, NULL);
// 	if (clientFd < 0)
// 		return ;

// 	pollfd pfd = {clientFd, POLLIN, 0};
// 	fcntl(clientFd, F_SETFL, O_NONBLOCK);
// 	_pollFds.push_back(pfd);
// 	_clients[clientFd] = User(clientFd);

// 	_pollFds[0].revents = 0;
// 	std::cout << "Client connected (fd: " << clientFd << ")." << std::endl;
// }

// void Server::_messageRoundabout(User& client, const Message& msg)
// {
// 	// _serverRoundabout[msg.getCommand()](client, msg);
// 	switch (Validator::commandMap[msg.getCommand()])
// 	{
// 	case PASS:
// 		pass_cmd(client, msg);
// 		break;
// 	case USER:
// 		user_cmd(client, msg);
// 		break;
// 	case NICK:
// 		nick_cmd(client, msg);
// 		break;
// 	case JOIN:
// 		break;
// 	case PART:
// 		_chanManager.part(client, msg);
// 		break;
// 	case TOPIC:
// 		break;
// 	case MODE:
// 		break;
// 	case KICK:
// 		break;
// 	case INVITE:
// 		break;
// 	case PRIVMSG:
// 		break;
// 	case NOTICE:
// 		break;
// 	default:
// 		break;
// 	}
// }

// void Server::broadcast(const std::string &msg, int senderFd)
// {
// 	for (t_clientMap::iterator it=_clients.begin(); it != _clients.end(); it++)
// 	{
// 		if (it->first == senderFd)
// 			it->second.pendingPush(msg);
// 	}
// }

// void Server::pass_cmd(User &client, const Message& msg)
// {
// 	std::cout << "getParams() => <" << msg.getParams() << ">" << std::endl;
// 	short perms = client.getPerms();
// 	if (perms == PERM_ALL)
// 		client.pendingPush(_rplGenerator.reply(462, client.getNickname()));
// 	else
// 		client.setPass(msg.getParams() == _password);
// }

// void Server::user_cmd(User &client, const Message& msg)
// {
// 	short perms = client.getPerms();
// 	if (perms == PERM_ALL)
// 		client.pendingPush(_rplGenerator.reply(462));
// 	else if (perms == PERM_NICK)
// 		client.pendingPush(_rplGenerator.reply(464));
// 	else
// 	{
// 		client.setUsername(msg.getParams());
// 		if (client.getPerms() == PERM_ALL)
// 			client.pendingPush(_rplGenerator.reply(1, client.getNickname(), client.getNickname()));
// 	}
// }

// void Server::nick_cmd(User &client, const Message& msg)
// {
// 	static const std::string leadCharBan = "#&:0123456789";

// 	short perms = client.getPerms();
// 	if (perms == PERM_USER)
// 	{
// 		client.pendingPush(_rplGenerator.reply(464));
// 		return;
// 	}

// 	const std::string& nick = msg.getParams();
// 	std::string oldNick = client.getNickname();
// 	if (_nickMap.count(nick) == 1)
// 		client.pendingPush(_rplGenerator.reply(433, oldNick, nick));
// 	else if (leadCharBan.find(nick[0]) != std::string::npos
// 			|| nick.find(' ') != std::string::npos)
// 		client.pendingPush(_rplGenerator.reply(432, oldNick, nick));
// 	else
// 	{
// 		_nickMap.erase(oldNick);
// 		_nickMap[nick] = client.getFd();
// 		client.setNickname(nick);
// 		std::string msg = ":" + oldNick + "!" + client.getUsername() + "@ft-irc NICK " + nick;
// 		broadcast(msg);
// 		if (perms == ~PERM_NICK)
// 			client.pendingPush(_rplGenerator.reply(1, nick, nick));
// 	}
// }

// void Server::privmsg_cmd(User &client, const Message &msg)
// {
// 	std::vector<std::string> targets(tokenize(msg.getParams(), ','));

// 	for (size_t i=0; i < targets.size(); i++)
// 	{
// 		if (targets[i][0] == '#')
// 			_chanManager.privmsg(client, targets[i], msg.getReply());
// 		else if (_nickMap.count(targets[i]) == 0)
// 			client.pendingPush(_rplGenerator.reply(401, targets[i]));
// 		else if (_nickMap[targets[i]] != client.getFd())
// 			_clients[_nickMap[targets[i]]].pendingPush(msg.getReply());
// 	}
// }
