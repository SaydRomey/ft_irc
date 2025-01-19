
#include "IRCClient.hpp"
#include <sstream>

/*
Constructs an IRCClient with the given socket and network address.
(Initializes the client as unauthenticated and sets up basic properties)
The client starts as unauthenticated, with an empty nickname and buffer.
*/
IRCClient::IRCClient(int socket, const struct sockaddr_in &address)
	: socket(socket), address(address), authenticated(false) {}

/*
Destroys the IRCClient instance.
Currently, this class does not allocate resources requiring explicit cleanup.
*/
IRCClient::~IRCClient(void) {}

/*
Returns the socket file descriptor associated with the client.
*/
int	IRCClient::getSocket(void) const
{
	return (socket);
}

/*
Returns whether the client has been authenticated.
*/
bool	IRCClient::isAuthenticated(void) const
{
	return (authenticated);
}

/*
Marks the client as authenticated.
*/
void	IRCClient::authenticate(void)
{
	authenticated = true;
}

/*
Sets the nickname for the client.
*/
void	IRCClient::setNickname(const std::string &nick)
{
	nickname = nick;
}

/*
Returns the client's nickname.
*/
const	std::string& IRCClient::getNickname(void) const
{
	return (nickname);
}

/*
Adds a channel to the list of channels the client has joined.
*/
void	IRCClient::addChannel(const std::string &channel)
{
	channels.push_back(channel);
}

/*
Returns the list of channels the client is currently a member of. (has joined)
*/
const std::vector<std::string>&	IRCClient::getChannels(void) constructor
{
	return (channels);
}

/*
Appends incomint data to the client's buffer.
Used for aggregating partial commands received from the client.
(To handle cases where a message or command is received in multiple parts)
*/
void	IRCClient::appendBuffer(const std::string &data)
{
	buffer += data;
}

/*
Retrieves the next complete command from the buffer, if one exists.
Commands in IRC are terminated by "\r\n".

Parameters:
    command - A reference to a string where the command will be stored.

Returns:
    true if a complete command was found and extracted.
    false if the buffer does not yet contain a complete command.
*/
bool	IRCClient::getNextCommand(std::string &command)
{
	// Find the position of the "\r\n" that marks the end of a command
	size_t	pos = buffer.find("\r\n"); // IRC messages end with \r\n

	if (pos != std::string::npos)
	{
		// Extract the command from the buffer
		command = buffer.substr(0, pos);

		// Remove the processed command (and the "\r\n") from the buffer
		buffer.erase(0, pos + 2);

		return (true); // A complete command was retrieved
	}
	return (false); // No complete command in the buffer
}
