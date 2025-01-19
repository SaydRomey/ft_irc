
#ifndef IRCCHANNEL_HPP
#define IRCCHANNEL_HPP

#include <string>
#include <map>

class IRCClient;

/*
The IRCChannel class represents an IRC channel, managing:
    - A list of clients currently in the channel.
    - Message broadcasting to all clients in the channel.
    - Adding and removing clients from the channel.
*/
class IRCChannel
{
	private:
		std::string					name;		// Name of the channel
		std::map<int, IRCClient*>	clients;	// Key: client socket FD, Value: IRCClient instance

	public:
		IRCChannel(const std::string &name);
		~IRCChannel(void);

		void	addClient(IRCClient *client);
		void	removeClient(int clientSock);
		void	broadcast(const std::string &message, int senderSock);
		bool	isEmpty(void) const;
};

#endif // IRCCHANNEL_HPP

#ifndef IRCCHANNEL_HPP
#define IRCCHANNEL_HPP

#include <string>
#include <map>
#include <set>

class IRCClient;

/*
The IRCChannel class represents an IRC channel, managing:
	- A list of clients in the channel.
	- Channel-specific features like invite-only mode, user limits, and topics.
	- Operator management and permissions.
*/
class IRCChannel
{
	private:
		std::string 				name;		// Name of the channel
		std::string					topic;		// Channel topic
		std::string					key;		// Channel password
		bool						inviteOnly;	// Whether the channel is invite-only
		int							userLimit;	// Maximum number of users allowed (-1 = no limit)
		std::map<int, IRCClient*>	clients;	// Key: client socket FD, Value: IRCClient instance
		std::set<int>				operators;	// Set of client socket FDs with operator privileges

	public:
		IRCChannel(const std::string &name);
		~IRCChannel(void);

		void	addClient(IRCClient *client);
		void	removeClient(int clientSock);
		void	broadcast(const std::string &message, int senderSock);
		bool	isEmpty(void) const;

		// Operator management
		void	addOperator(int clientSock);
		void	removeOperator(int clientSock);
		bool	isOperator(int clientSock) const;

		// Channel modes
		void	setInviteOnly(bool enabled);
		void	setKey(const std::string &key);
		void	setUserLimit(int limit);
		void	setTopic(const std::string &newTopic, int clientSock);

		// Mode checks
		bool	isInviteOnly(void) const;
		const std::string&	getKey(void) const;
		int		getUserLimit(void) const;
		const std::string&	getTopic(void) const;
		int		getUserCount(void) const;

		// Invite management
		bool	inviteClient(int inviterSock, IRCClient* client);
};

#endif // IRCCHANNEL_HPP
