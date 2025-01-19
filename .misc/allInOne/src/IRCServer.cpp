
#include "IRCServer.hpp"
#include <iostream>
#include <sstream>

/*
Constructs an IRCServer instance with the specified port and password.
Initializes the server socket, binds it to the given port, and sets it to listen.
*/
IRCServer::IRCServer(const std::string &port, const std::string &password) : password(password)
{
	serverSock = NetworkUtils::createSocket();
	NetworkUtils::setSocketOption(serverSock, SOL_SOCKET, SO_REUSEADDR, 1);
	NetworkUtils::bindSocket(serverSock, "0.0.0.0", std::atoi(port.c_str()));
	NetworkUtils::setNonBlocking(serverSock);
	NetworkUtils::listenSocket(serverSock);
	std::cout << "IRCServer initialized on port " << port << "\n";
}

/*
Destroys the IRCServer instance.
Closes the server socket, removes all clients and channels, and releases resources.
*/
IRCServer::~IRCServer(void)
{
	std::map<int, IRCClient*>::iterator itClient = clients.begin();

	// Close client sockets and delete client objects
	while (itClient != clients.begin())
	{
		NetworkUtils::closeSocket(itClient->first);
		delete (itClient->second);
		++itClient;
	}

	// Close the server socket
	NetworkUtils::closeSocket(serverSock);

	// Delete all channel objects
	std::map<std::string, IRCChannel*>::iterator itServer = channels.begin();
	while (itServer != channels.end())
	{
		delete (itServer->second);
		++itServer;
	}
}

/*
Starts the server's main event loop.
Uses poll() to monitor the server socket and client sockets for events.
Handles incoming connections and client messages.
*/
void	IRCServer::start(void)
{
	struct pollfd fds[1024]; // Array to monitor up to 1024 connections
	fds[0].fd = serverSock;
	fds[0].events = POLLIN;

	while (true)
	{
		// Populate pollfd array with client sockets
		int	nfds = 1;
		std::map<int, IRCClient*>::iterator it = clients.begin();

		while (it != clients.end())
		{
			fds[nfds].fd = it->first;
			fds[nfds].events = POLLIN;
			++nfds;
			++it;
		}

		// Wait for events using poll()
		int	pollCount = poll(fds, nfds, -1);
		if (pollCount < 0)
		{
			NetworkUtils::handleError("Polling error");
		}

		// Check for events on the server socket (new connections)
		if (fds[0].revents & POLLIN)
		{
			handleNewConnection();
		}

		// Check for events on client sockets (incoming messages)
		int	i = 1;
		while (i < nfds)
		{
			if (fds[i].revents & POLLIN)
			{
				handleClientMessage(fds[i].fd);
			}
			++i;
		}
	}
}

/*
Handles a new incoming client connection.
Accepts the connection, sets it to non-blocking mode, and adds the client to the client map.
*/
void	IRCServer::handleNewConnection(void)
{
	struct sockaddr_in	clientAddr;
	int	clientSock = NetworkUtils::acceptConnection(serverSock, clientAddr);

	// Set the client socket to non-blocking mode
	NetworkUtils::setNonBlocking(clientSock);

	// Create a new IRCClient instance and store it
	clients[clientSock] = new IRCClient(clientSock, clientAddr);

	std::cout << "New client connected: " << clientSock << "\n";
}

/*
Handles messages received from a client.
Aggregates partial data into complete commands and processes them.
*/
void	IRCServer::handleClientMessage(int clientSock)
{
	IRCClient	*client = clients[clientSock];

	try
	{
		// Receive data and append it to the client's buffer
		std::string	message = NetworkUtils::receiveData(clientSock);
		client->appendBuffer(message);

		// Process each complete command in the buffer
		std::string	command;
		while (client->getNextCommand(command))
		{
			processCommand(client, command);
		}
	}
	catch (const std::exception &e)
	{
		// Handle client disconnection
		std::cerr << "Client " << clientSock << " disconnected: " << e.what() << "\n";
		removeClient(clientSock);
	}
}

/*
Removes a client from the server.
Handles client cleanup, including channel membership and resources.
*/
void	IRCServer::removeClient(int clientSock)
{
	IRCClient	*client = clients[clientSock];

	if (client)
	{
		// Remove the client from all joined channels
		std::vector<std::string>::iterator it = client->getChannels().begin();
		while ( it != client->getChannels().end())
		{
			IRCChannel	*channel = channels[*it];

			if (channel)
			{
				channel->removeClient(clientSock);
				if (channel->isEmpty())
				{
					delete (channel);
					channels.erase(*it);
				}
			}
			++it;
		}

		// Delete the client object and remove it from the client map
		delete (client);
		clients.erase(clientSock);
	}

	// Close the client socket
	NetworkUtils::closeSocket(clientSock);
}

/*
Broadcasts a message to all members of a specified channel.

Parameters:
	channelName - The name of the channel to broadcast to.
	message - The message to be sent.
	senderSock - The socket file descriptor of the sender (excluded from the broadcast).

If the channel does not exist, the function does nothing.
*/
void	IRCServer::broadcastToChannel(const std::string &channelName, const std::string &message, int senderSock)
{
	// Find the channel by name
	IRCChannel	*channel = channels[channelName];

	// If the channel exists, broadcast the message
	if (channel)
	{
		channel->broadcast(message, senderSock);
	}
}

/*
Processes a command received from a client.
Executes actions based on the command type (e.g., PASS, NICK, JOIN, PRIVMSG).
*/
void	IRCServer::processCommand(IRCClient *client, const std::string &command)
{
	std::istringstream	ss(command);
	std::string	cmd, arg1, arg2;
	ss >> cmd >> arg1 >> arg2;

	if (cmd == "PASS")
	{
		if (arg1 == password)
		{
			client->authenticate();
			NetworkUtils::sendData(client->getSocket(), "Password accepted.\n");
		}
		else
		{
			NetworkUtils::sendData(client->getSocket(), "Incorrect password.\n");
			removeClient(client->getSocket());
		}
	}
	else if (cmd == "NICK")
	{
		client->setNickname(arg1);
	}
	else if (cmd == "JOIN")
	{
		if (channels.find(arg1) == channels.end())
		{
			channels[arg1] = new IRCChannel(arg1);
		}
		channels[arg1]->addClient(client);
		client->addChannel(arg1);
	}
	else if (cmd == "PRIVMSG")
	{
		broadcastToChannel(arg1, client->getNickname() + ": " + arg2, client->getSocket());
	}
	// Extend this function to include additional commands like KICK, INVITE, etc.
}

/*
IRCServer server("6667", "password123");
server.start();
*/

///// ** Bigger processCommand **

/*
Processes a command received from a client.
Executes the appropriate action based on the command type.

Parameters:
	client - The IRCClient that sent the command.
	command - The full command string sent by the client.

Supported commands:
	- PASS <password>: Authenticate the client.
	- NICK <nickname>: Set the client's nickname.
	- JOIN <channel>: Join the specified channel.
	- PRIVMSG <channel> <message>: Send a private message to a channel.
	- KICK <channel> <nickname>: Remove a user from a channel (operator only).
	- INVITE <nickname> <channel>: Invite a user to a channel.
	- TOPIC <channel> <topic>: Set or view the channel topic.
	- MODE <channel> [mode] [parameters]: Modify channel settings (operator only).
	- BOT <command>: Interact with a bot to process specific commands.
	- UPLOAD <fileName>: Handle file upload from a client.
	- DOWNLOAD <fileName>: Handle file download to a client.
*/
void	IRCServer::processCommand(IRCClient *client, const std::string &command)
{
	// Parse the command into its components
	std::istringstream ss(command);
	std::string cmd, arg1, arg2, arg3;
	ss >> cmd >> arg1 >> arg2 >> arg3;

	if (cmd == "PASS")
	{
		// Authenticate the client
		if (arg1 == password)
		{
			client->authenticate();
			NetworkUtils::sendData(client->getSocket(), "Password accepted.\n");
		}
		else
		{
			NetworkUtils::sendData(client->getSocket(), "Incorrect password.\n");
			removeClient(client->getSocket());
		}
	}
	else if (cmd == "NICK")
	{
		// Set the client's nickname
		client->setNickname(arg1);
	}
	else if (cmd == "JOIN")
	{
		// Join a channel, creating it if it doesn't exist
		if (channels.find(arg1) == channels.end())
		{
			channels[arg1] = new IRCChannel(arg1);
		}
		channels[arg1]->addClient(client);
		client->addChannel(arg1);
	}
	else if (cmd == "PRIVMSG")
	{
		// Send a private message to a channel
		broadcastToChannel(arg1, client->getNickname() + ": " + arg2, client->getSocket());
	}
	else if (cmd == "KICK")
	{
		// KICK <channel> <nickname>
		// Remove a specified user from a channel (requires operator privileges).
		IRCChannel	*channel = channels[arg1]; // // Get the specified channel by name.

		// Check if the channel exists and the client is an operator in the channel.
		if (channel && channel->isOperator(client->getSocket()))
		{
			// Iterate through the list of connected clients to find the target user by nickname.
			std::map<int, IRCClient*>::iterator itClient = clients.begin();
			while (itClient != clients.end())
			{
				// Match the nickname with the target user.
				if (itClient->second->getNickname() == arg2)
				{
					// Remove the target user from the channel.
					channel->removeClient(itClient->first);
					// Notify the kicked user.
					NetworkUtils::sendData(itClient->first, "You have been kicked from " + arg1 + ".\r\n");
					return ; // Exit after successfully processing the command.
				}
				++itClient;
			}
			// If the target user is not found in the client list, notify the sender.
			NetworkUtils::sendData(client->getSocket(), "User not found.\r\n");
		}
		else
		{
			// Notify the sender if they are not an operator or the channel doesn't exist.
			NetworkUtils::sendData(client->getSocket(), "You are not an operator in this channel.\r\n");
		}
	}
	else if (cmd == "INVITE")
	{
		// INVITE <nickname> <channel>
		// Invite a specified user to join a channel.
		IRCChannel	*channel = channels[arg2]; // Get the specified channel by name.

		if (channel)
		{
			// Iterate through the list of connected clients to find the target user by nickname.
			std::map<int, IRCClient*>::iterator itServer = clients.begin();
			while( itServer != clients.end())
			{
				// Match the nickname with the target user.
				if (itServer->second->getNickname() == arg1)
				{
					// Send an invite to the target user.
					channel->inviteClient(client->getSocket(), itServer->second);
					return ; // Exit after successfully processing the command.
				}
				++ititServer;
			}
			// If the target user is not found in the client list, notify the sender.
			NetworkUtils::sendData(client->getSocket(), "User not found.\r\n");
		}
		else
		{
			NetworkUtils::sendData(client->getSocket(), "Channel not found.\r\n");
		}
	}
	else if (cmd == "TOPIC")
	{
		// TOPIC <channel> <topic>
		// Set or view the topic of a specified channel.
		IRCChannel	*channel = channels[arg1];

		if (channel)
		{
			// Update the channel's topic (handled by the IRCChannel class).
			channel->setTopic(arg2, client->getSocket());
		}
		else
		{
			// Notify the sender if the channel doesn't exist.
			NetworkUtils::sendData(client->getSocket(), "Channel not found.\r\n");
		}
	}
	else if (cmd == "MODE")
	{
		// MODE <channel> [mode] [parameters]
		// Modify the settings of a channel (requires operator privileges).
		IRCChannel	*channel = channels[arg1]; // Get the specified channel by name.

		// Check if the channel exists and the client is an operator in the channel.
		if (channel && channel->isOperator(client->getSocket()))
		{
			if (arg2 == "+i")
			{
				// Set the channel to invite-only mode.
				channel->setInviteOnly(true);
				NetworkUtils::sendData(client->getSocket(), "Channel set to invite-only.\r\n");
			}
			else if (arg2 == "-i")
			{
				// Remove the invite-only restriction from the channel.
				channel->setInviteOnly(false);
				NetworkUtils::sendData(client->getSocket(), "Invite-only removed.\r\n");
			}
			else if (arg2 == "+k")
			{
				// Set a password (key) for the channel.
				channel->setKey(arg3);
				NetworkUtils::sendData(client->getSocket(), "Channel key set.\r\n");
			}
			else if (arg2 == "+l")
			{
				// Set a user limit for the channel.
				channel->setUserLimit(std::atoi(arg3.c_str()));
				NetworkUtils::sendData(client->getSocket(), "User limit set to " + arg3 + ".\r\n");
			}
		}
		else
		{
			// Notify the sender if the mode command is invalid or unsupported.
			// Notify the sender if they are not an operator or the channel doesn't exist. ?
			NetworkUtils::sendData(client->getSocket(), "You are not an operator in this channel.\r\n");
		}
	}
	else if (cmd == "BOT")
	{
		// BOT <command>
		// Initialize a bot with a specific nickname
		IRCBot	bot("HelperBot");

		// Process the command using the bot
		std::string	response = bot.handleCommand(arg1);

		// Send the bot's response back to the client
		NetworkUtils::sendData(client->getSocket(), response + "\r\n");
	}
	else if (cmd == "UPLOAD")
	{
		// UPLOAD <fileName>
		// Handle the start of a file download from the client to the server.
		IRCFileTransfer	transfer;

		if (transfer.startDownload(arg1))
		{
			// Notify the client that the server is ready to receive the file.
			NetworkUtils::sendData(client->getSocket(), "Ready to receive file.\r\n");
		}
		else
		{
			// Notify the client that the file could not be opened for writing.
			NetworkUtils::sendData(client->getSocket(), "Failed to open file.\r\n");
		}
	}
	else if (cmd == "DOWNLOAD")
	{
		// DOWNLOAD <fileName>
		// Handle the start of a file upload from the server to the client.
		IRCFileTransfer	transfer;

		if (transfer.startUpload(arg1))
		{
			// Continuously read and send chunks of the file until the upload is complete.
			while (!transfer.isUploadComplete())
			{
				std::string	chunk = transfer.readChunk();
				NetworkUtils::sendData(client->getSocket(), chunk);
			}

			// Notify the client that the file transfer is complete.
			NetworkUtils::sendData(client->getSocket(), "File transfer complete.\r\n");
	}
	else
	{
		// Notify the client that the file could not be opened for reading.
		NetworkUtils::sendData(client->getSocket(), "Failed to open file.\r\n");
	}
}
	else
	{
		// Unknown command
		NetworkUtils::sendData(client->getSocket(), "Unknown command.\r\n");
	}
}

// bot

#include "IRCBot.hpp"



// file transfer

#include "IRCFileTransfer.hpp"



//


