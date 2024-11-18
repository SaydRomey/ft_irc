
#ifndef SERVER_HPP
# define SERVER_HPP

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
# include "MessageAggregator.hpp"
# include "Parser.hpp"

class Server
{
	public:
		Server(const std::string &port, const std::string &password);
		~Server(void);

		void	start(void);
		void	stop(void);
	
	private:
		// server core
		int	_serverFd;
		std::string	_port;
		std::string	_password;
		bool	_isRunning;

		// polling
		std::vector<pollfd>	_pollFds;
		std::map<int, std::string>	_clients; // maps file descriptor to client identifier (e.g., nicknames)

		// parsing and buffering
		Aggregator	_aggregator;
		Parser	_parser;

		// methods
		void	_initSocket(void);
		void	_acceptConnection(void);
		void	_handleClient(int clientFd);
		void	_broadcast(const std::string &message, int senderFd = -1);

		// command handlers
		void	_handleCommand(int clientFd, const Message &message);
		void	_authenticateClient(int clientFd, const Message &message);

		// helpers
		void	_sendMessage(int cliendFd, const std::string &message);
		void	_disconnectClient(int clientFd, const std::string &reason);
};

#endif // SERVER_HPP
