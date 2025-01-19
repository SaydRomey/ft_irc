
/*

idea for broadcasting a message to all channel members:

Server as the Orchestrator:
The Server class acts as the central hub
for managing channels, users, and interactions between them.

Channel and User Responsibilities:
The Channel class maintains a list of users in the channel.
The User class represents a single user
and handles sending messages to its client socket.

Broadcast Logic in the Server:
When a user joins a channel,
the Server instructs the Channel to broadcast a message to all its users,
except the one who just joined.
*/

void	Server::broadcastToChannel(const std::string &channelName, const std::string &message, int excludeFd)
{
	Channel *channel = getChannel(channelName); // Retrieve the channel object
	if (!channel)
	{
		return ; // Channel does not exist, do nothing
	}

	// Loop through all users in the channel and send the message
	const std::vector<User *> &users = channel->getUsers();

	size_t	i = 0;
	while (i < users.size())
	{
		User *user = users[i];
		if (user->getFd() != excludeFd) // Exclude the user who just joined
		{
			user->sendMessage(message);
		}
		++i;
	}
}

/*
Supporting Classes
Channel Class
The Channel class manages its list of users and provides an interface to access them:
*/

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "User.hpp"
# include <string>
# include <vector>

class Channel
{
	public:
		Channel(const std::string &name);
		~Channel();

		const std::string &getName() const;
		const std::vector<User *> &getUsers() const;

		void addUser(User *user);
		void removeUser(User *user);

	private:
		std::string _name;
		std::vector<User *> _users; // List of users in the channel
};

#endif // CHANNEL_HPP

#include "Channel.hpp"
#include <algorithm>

Channel::Channel(const std::string &name) : _name(name) {}
Channel::~Channel() {}

const std::string &Channel::getName() const
{
	return (_name);
}

const std::vector<User *> &Channel::getUsers() const
{
	return (_users);
}

void Channel::addUser(User *user)
{
	_users.push_back(user);
}

void Channel::removeUser(User *user)
{
	_users.erase(std::remove(_users.begin(), _users.end(), user), _users.end());
}

/*
User Class
The User class represents an individual user and provides functionality to send messages to the client:
*/

#ifndef USER_HPP
#define USER_HPP

# include <string>

class User
{
	public:
		User(int fd, const std::string &nickname);
		~User();

		int getFd() const;
		const std::string &getNickname() const;

		void sendMessage(const std::string &message) const;

	private:
		int _fd; // File descriptor for the user's socket
		std::string _nickname;
};

#endif // USER_HPP

#include "User.hpp"
#include <unistd.h>
#include <cstring>

User::User(int fd, const std::string &nickname) : _fd(fd), _nickname(nickname) {}
User::~User() {}

int User::getFd() const
{
	return (_fd);
}

const std::string &User::getNickname() const
{
	return (_nickname);
}

void User::sendMessage(const std::string &message) const
{
	write(_fd, message.c_str(), message.size());
}

/*
Server Class
The Server class manages channels and users:
*/
#ifndef SERVER_HPP
#define SERVER_HPP

# include "Channel.hpp"
# include "User.hpp"
# include <map>
# include <string>

class Server
{
	public:
		Server();
		~Server();

		Channel *getChannel(const std::string &name);
		void addChannel(const std::string &name);
		void broadcastToChannel(const std::string &channelName, const std::string &message, int excludeFd);

	private:
		std::map<std::string, Channel *> _channels; // Map of channel names to Channel objects
};

#endif // SERVER_HPP

#include "Server.hpp"

Server::Server() {}
Server::~Server()
{
	std::map<std::string, Channel *>::iterator it = _channels.begin();

	while (it != _channels.end())
	{
		delete it->second;
		++it;
	}
}

Channel *Server::getChannel(const std::string &name)
{
	std::map<std::string, Channel *>::iterator it = _channels.find(name);

	if (it != _channels.end())
	{
		return (it->second);
	}
	return (NULL);
}

void Server::addChannel(const std::string &name)
{
	if (_channels.find(name) == _channels.end())
	{
		_channels[name] = new Channel(name);
	}
}

void Server::broadcastToChannel(const std::string &channelName, const std::string &message, int excludeFd)
{
	Channel *channel = getChannel(channelName);

	if (!channel)
	{
		return ;
	}

	const std::vector<User *> &users = channel->getUsers();

	size_t i = 0;
	while (i < users.size())
	{
		User *user = users[i];
		if (user->getFd() != excludeFd)
		{
			user->sendMessage(message);
		}
		++i;
	}
}

void Server::broadcastToChannel(const std::string &channelName, const std::string &senderNickname, const std::string &message, int excludeFd)
{
	Channel *channel = getChannel(channelName);
	
	if (!channel)
	{
		return ; // Channel does not exist
	}

	// Use the Reply class to generate the JOIN message
	Reply replyGenerator;
	std::string joinReply = replyGenerator.generateReply(Reply::RPL_CHANNEL_MSG, makeArgs(senderNickname, channelName, message));

	// Send the reply to all users in the channel except the sender
	const std::vector<User *> &users = channel->getUsers();

	size_t i = 0;
	while (i < users.size())
	{
		User *user = users[i];
		if (user->getFd() != excludeFd)
		{
			user->sendMessage(joinReply);
		}
		++i;
	}
}

/*
Putting It All Together
When a user joins a channel, the Server class can handle the process like this:
*/
void Server::userJoinChannel(User *user, const std::string &channelName)
{
	// Create the channel if it doesn't exist
	if (!getChannel(channelName))
	{
		addChannel(channelName);
	}

	Channel *channel = getChannel(channelName);

	// Add the user to the channel
	channel->addUser(user);

	// Notify all other users in the channel
	std::string joinMessage = ":" + user->getNickname() + " JOIN " + channelName;
	broadcastToChannel(channelName, joinMessage, user->getFd());
}

void Server::userJoinChannel(User *user, const std::string &channelName)
{
	// Create the channel if it doesn't exist
	if (!getChannel(channelName))
	{
		addChannel(channelName);
	}

	Channel *channel = getChannel(channelName);

	// Add the user to the channel
	channel->addUser(user);

	// Use the Reply class to generate the JOIN message
	Reply replyGenerator;
	std::string joinMessage = replyGenerator.generateReply(Reply::RPL_CHANNEL_MSG, makeArgs(user->getNickname(), channelName, "has joined the channel."));

	// Broadcast the JOIN message to all other users
	broadcastToChannel(channelName, user->getNickname(), "has joined the channel.", user->getFd());
}


#include "Server.hpp"
#include "User.hpp"
#include <iostream>

int main()
{
	Server server;

	// Create example users
	User user1(1, "ced");
	User user2(2, "j-l");
	User user3(3, "nad");

	// Users join the channel
	server.userJoinChannel(&user1, "#general");
	server.userJoinChannel(&user2, "#general");
	server.userJoinChannel(&user3, "#general");

	return (0);
}
