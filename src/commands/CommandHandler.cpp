
// #include "CommandHandler.hpp"

// CommandHandler::CommandHandler(ClientManager &clients, ChannelManager &channels) \
// : _clients(clients), _channels(channels) {}

// CommandHandler::~CommandHandler(void) {}

// void	CommandHandler::handleCommand(int clientFd, const Message &message)
// {
// 	const std::string	&command = message.getCommand();

// 	if (_clients.isAuthenticated(clientFd) || command == "PASS" || command == "NICK" || command == "USER")
// 	{
// 		if (_clients.isAuthenticated(clientFd) || MessageValidator::isAuthenticationCommand(command))
// 		{
// 			_handleAuthentication(clientFd, message);
// 		}
// 		else if (MessageValidator::isMessagingCommand(command))
// 		{
// 			_handleMessaging(clientFd, message);
// 		}
// 		else if (MessageValidator::isChannelManagementCommand(command))
// 		{
// 			_handleChannelManagement(clientFd, message);
// 		}
// 		else if (MessageValidator::isChannelOperatorCommand(command))
// 		{
// 			_handleOperatorCommand(clientFd, message);
// 		}
// 		else
// 		{
// 			_clients.sendMessage(clientFd, "ERROR: Unknown command.");
// 		}
// 	}
// 	else
// 	{
// 		_clients.sendMessage(clientFd, "ERROR: You must authenticate first.");
// 	}
// }

// void	CommandHandler::_handleAuthentication(int clientFd, const Message &message)
// {
// 	// Implement handling for PASS, NICK, and USER
// }

// void	CommandHandler::_handleMessaging(int clientFd, const Message &message)
// {
// 	// Implement handling for PRIVMSG and NOTICE
// }

// void	CommandHandler::_handleChannelManagement(int clientFd, const Message &message)
// {
// 	// Implement handling for JOIN, PART
// }

// void	CommandHandler::_handleOperatorCommand(int clientFd, const Message &message)
// {
// 	// Implement handling for KICK, INVITE, TOPIC, MODE
// }
