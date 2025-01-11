
#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

# include "NetworkUtils.hpp"
# include "IRCClient.hpp"
# include "IRCChannel.hpp"
# include <map>
# include <vector>
# include <string>

/*
The IRCServer class manages the lifecycle of an IRC server.
It handles:
	- Socket setup and network communication
	- Authentication and client connections
	- Channel and message management
	- Command parsing and execution
*/
class IRCServer
{
	private:
		int									serverSock;	// Server socket file descriptor
		std::string							password;	// Password for client authentication
		std::map<int, IRCClient*>			clients;	// Key: client socket FD, Value: IRCClient instance
		std::map<std::string, IRCChannel*>	channels;	// Key: channel name, Value: IRCChannel instance

	public:
		IRCServer(const std::string &port, const std::string &password);
		~IRCServer(void);

		void	start(void);
		void	handleNewConnection(void);
		void	handleClientMessage(int clientSock);
		void	removeClient(int clientSock);
		void	broadcastToChannel(const std::string &channelName, const std::string &message, int senderSock);
		void	processCommand(IRCClient *client, const std::string &command);
};

#endif // IRCSERVER_HPP
