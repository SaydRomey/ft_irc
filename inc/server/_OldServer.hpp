
#ifndef _OLDSERVER_HPP
# define _OLDSERVER_HPP

# include <string>
# include <map>
# include <vector>
# include <set>
# include <queue>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <poll.h>
# include <stdexcept>
# include <cerrno>
# include "Aggregator.hpp"
# include "Parser.hpp"
# include "Client.hpp"

namespace ft
{

class Server
{
	public:
		Server(const std::string &port, const std::string &password);
		~Server(void);

		void	start(void);
		void	stop(void);
	
	private:
		// server core
		int			_serverFd;
		std::string	_port;
		std::string	_password;
		bool		_isRunning;
		time_t		_startTime;

		// polling
		std::vector<pollfd>	_pollFds;
		std::map<int, ft::Client>	_clients; // maps file descriptor to ft::Client object

		// parsing and buffering
		Aggregator	_aggregator;
		Parser	_parser;

		// methods
		void	_initSocket(void);
		void	_acceptConnection(void);
		void	_handleClient(int clientFd);
		void	_broadcast(const std::string &message, int senderFd = -1);
		void	_welcomeClient(int clientFd);

		// command handlers
		void	_handleCommand(int clientFd, const Message &message);
		void	_authenticateClient(int clientFd, const Message &message);

		// helpers
		void	_sendMessage(int clientFd, const std::string &message);
		void	_disconnectClient(int clientFd, const std::string &reason);

		// template<int clientFd>
		// static bool _is_clientFd(const pollfd &pfd) { return pfd.fd == clientFd; }
};

}

#endif
