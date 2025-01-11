

#include "IRCChannel.hpp"
#include "IRCClient.hpp"
#include "NetworkUtils.hpp"
#include <iostream>

/*
Constructs an IRCChannel with the given name.
Initializes channel settings to their defaults.
*/
IRCChannel::IRCChannel(const std::string &name) : name(name) {}

/*
Destroys the IRCChannel instance.
Currently, no special cleanup is required.
*/
IRCChannel::~IRCChannel(void) {}

/*
Adds a client to the channel.
Notifies all other clients in the channel that a new user has joined.
*/
void	IRCChannel::addClient(IRCClient *client)
{
	// Add the client to the channel's client map using the client's socket as the key.
	clients[client->getSocket()] = client;

	// Broadcast a join message to all other clients
	// Notify other clients in the channel about the new user's arrival.
	broadcast("User " + client->getNickname() + " has joined the channel.", client->getSocket());
}

/*
Removes a client from the channel.
Notifies all other clients that the user has left.
*/
void	IRCChannel::removeClient(int clientSock)
{
	// Find the client in the channel using the socket file descriptor.
	IRCClient	*client = clients[clientSock];

	if (client)
	{
		// Notify other clients that the user has left the channel.
		broadcast("User " + client->getNickname() + " has left the channel.", clientSock);

		// Remove the client from the client map
		clients.erase(clientSock);
	}
}

/*
Broadcasts a message to all clients in the channel except the sender.
Parameters:
	message - The message to broadcast.
	senderSock - The socket FD of the sender (excluded from receiving the message).
*/
void	IRCChannel::broadcast(const std::string &message, int senderSock)
{
	std::map<int, IRCClient*>::iterator	it = clients.begin();

	// Iterate through all clients in the channel.
	while (it != clients.end())
	{
		// Skip the sender of the message.
		if (it->first != senderSock)
		{
			try
			{
				// Send the message to the current client.
				NetworkUtils::sendData(it->first, message + "\r\n");
			}
			catch (const std::exception &e)
			{
				// Log an error if sending the message fails.
				std::cerr << "Failed to send message to client " << it->first << ": " << e.what() << "\n";
			}
		}
		++it;
	}
}

/*
Returns true if the channel has no clients; otherwise, false.
*/
bool	IRCChannel::isEmpty(void) const
{
	// Check if the client map is empty.
	return (clients.empty());
}

/////

#include "IRCChannel.hpp"
#include "IRCClient.hpp"
#include "NetworkUtils.hpp"

/*
Constructs an IRCChannel with the given name.
Initializes channel settings to their defaults.
*/
IRCChannel::IRCChannel(const std::string &name)
	: name(name), inviteOnly(false), userLimit(-1) {}

/*
Destroys the IRCChannel instance.
*/
IRCChannel::~IRCChannel(void) {}

/*
Adds a client to the channel.
Checks for user limits and sends notifications if the channel is full.
*/
void	IRCChannel::addClient(IRCClient *client)
{
	// If the channel has a user limit and it's reached, notify the client and reject the join.
	if (userLimit != -1 && getUserCount() >= userLimit)
	{
		NetworkUtils::sendData(client->getSocket(), "Channel is full.\r\n");
		return ;
	}

	// Add the client to the channel's client map using the client's socket as the key.
	clients[client->getSocket()] = client;

	// Notify other clients in the channel that a new user has joined.
	broadcast("User " + client->getNickname() + " has joined the channel.", client->getSocket());
}

/*
Removes a client from the channel.
If the client is an operator, they are removed from the operator list as well.
*/
void	IRCChannel::removeClient(int clientSock)
{
	// Remove the client from the client map.
	clients.erase(clientSock);

	// Remove the client from the operator list if they are an operator.
	operators.erase(clientSock);

	// Notify other clients in the channel that the user has left.
	broadcast("User has left the channel.", clientSock);
}

/*
Broadcasts a message to all clients in the channel except the sender.

Parameters:
	message - The message to broadcast.
	senderSock - The socket FD of the sender (excluded from receiving the message).
*/
void	IRCChannel::broadcast(const std::string &message, int senderSock)
{
	std::map<int, IRCClient*>::iterator	it = clients.begin();

	// Iterate through all clients in the channel.
	while (it != clients.end())
	{
		// Skip the sender of the message.
		if (it->first != senderSock)
		{
			try
			{
				// Send the message to the current client.
				NetworkUtils::sendData(it->first, message + "\r\n");
			}
			catch (const std::exception &e)
			{
				// Log an error if sending the message fails.
				std::cerr << "Failed to send message to client " << it->first << ": " << e.what() << "\n";
			}
		}
		++it;
	}
}

/*
Returns whether the channel is empty (i.e., has no clients).
*/
bool	IRCChannel::isEmpty(void) const
{
	// Check if the client map is empty.
	return (clients.empty());
}

/*
Adds a client to the list of operators.
*/
void	IRCChannel::addOperator(int clientSock)
{
	// Add the client's socket to the operator set.
	operators.insert(clientSock);
}

/*
Removes a client from the list of operators.
*/
void	IRCChannel::removeOperator(int clientSock)
{
	// Remove the client's socket from the operator set.
	operators.erase(clientSock);
}

/*
Returns true if the client is an operator in the channel.
*/
bool	IRCChannel::isOperator(int clientSock) const
{
	// Check if the client's socket is in the operator set.
	return (operators.find(clientSock) != operators.end());
}

/*
Enables or disables invite-only mode for the channel.
*/
void	IRCChannel::setInviteOnly(bool enabled)
{
	// Set the invite-only flag to the provided value.
	inviteOnly = enabled;
}

/*
Sets the password (key) for the channel.
*/
void	IRCChannel::setKey(const std::string &key)
{
	// Store the provided key for the channel.
	this->key = key;
}

/*
Sets the maximum number of users allowed in the channel.
*/
void	IRCChannel::setUserLimit(int limit)
{
	// Set the user limit to the provided value.
	userLimit = limit;
}

/*
Sets the topic of the channel.
Only operators can change the topic.
*/
void	IRCChannel::setTopic(const std::string &newTopic, int clientSock)
{
	// Check if the client is an operator before allowing the topic change.
	if (isOperator(clientSock))
	{
		// Set the new topic and notify all clients in the channel.
		topic = newTopic;
		broadcast("Channel topic has been changed to: " + topic, -1);
	}
	else
	{
		// Notify the client that they lack permissions to change the topic.
		NetworkUtils::sendData(clientSock, "Only operators can change the topic.\r\n");
	}
}

/*
Returns whether the channel is invite-only.
*/
bool	IRCChannel::isInviteOnly(void) const
{
	// Return the value of the invite-only flag.
	return (inviteOnly);
}

/*
Returns the password (key) of the channel.
*/
const std::string&	IRCChannel::getKey() const
{
	// Return the channel's password.
	return (key);
}

/*
Returns the maximum user limit of the channel.
*/
int	IRCChannel::getUserLimit(void) const
{
	// Return the user limit.
	return (userLimit);
}

/*
Returns the topic of the channel.
*/
const std::string&	IRCChannel::getTopic(void) const
{
	// Return the current topic.
	return (topic);
}

/*
Returns the current number of users in the channel.
*/
int	IRCChannel::getUserCount(void) const
{
	// Return the number of clients in the channel's client map.
	return (clients.size());
}

/*
Invites a client to the channel.
Only operators can send invites.
*/
bool	IRCChannel::inviteClient(int inviterSock, IRCClient *client)
{
	// Check if the inviter is an operator.
	if (isOperator(inviterSock))
	{
		// Add the invited client to the channel.
		addClient(client);

		// Notify the invited client.
		NetworkUtils::sendData(client->getSocket(), "You have been invited to join channel " + name + ".\r\n");
		return (true);
	}
	else
	{
		// Notify the inviter that they lack permissions to invite users.
		NetworkUtils::sendData(inviterSock, "Only operators can invite users.\r\n");
		return (false);
	}
}
