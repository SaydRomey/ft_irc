
// #ifndef COMMANDHANDLER_HPP
// # define COMMANDHANDLER_HPP

// # include <string>
// # include "Channel.hpp"
// # include "Message.hpp"

// class CommandHandler
// {
// 	public:
// 		CommandHandler(ClientManager &clients, ChannelManager &channels);
// 		~CommandHandler(void);

// 		void	handleCommand(int clientFd, const Message &message);
	
// 	private:
// 		ClientManager &_clients;
// 		ChannelManager &_channels;

// 		void	_handleAuthentication(int clientFd, const Message &message);
// 		void	_handleMessaging(int clientFd, const Message &message);
// 		void	_handleChannelManagement(int clientFd, const Message &message);
// 		void	_handleOperatorCommand(int clientFd, const Message &message);
// };

// #endif // COMMANDHANDLER_HPP
