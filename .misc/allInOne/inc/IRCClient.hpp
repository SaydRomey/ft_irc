
#ifndef IRCCLIENT_HPP
#define IRCCLIENT_HPP

#include <string>
#include <vector>
#include <netinet/in.h> // struct sockaddr_in

/*
The IRCClient class represents an individual client connected to the IRC server.
It handles:
	- Client-specific data like socket, nickname, and authentication status.
	- Channel memberships for the client.
	- Command parsing from the client's incoming data.
*/
class IRCClient
{
	private:
		int							socket; // Client's socket file descriptor
		struct sockaddr_in			address; // Client's network address
		bool						authenticated; // Authentication status of the client
		std::string					nickname; // Client's nickname
		std::vector<std::string>	channels; // List of channels the client has joined
		std::string					buffer; // Buffer for processing partial data

	public:
		IRCClient(int socket, const struct sockaddr_in &address);
		~IRCClient(void);

		int		getSocket(void) const;
		bool	isAuthenticated(void) const;
		void	authenticate(void);
		void	setNickname(const std::string &nick);
		const std::string&	getNickname(void) const;
		void	addChannel(const std::string &channel);
		const std::vector<std::string>&	getChannels(void) const;

		void	appendBuffer(const std::string &data);
		bool	getNextCommand(std::string &command);
};

#endif // IRCCLIENT_HPP
