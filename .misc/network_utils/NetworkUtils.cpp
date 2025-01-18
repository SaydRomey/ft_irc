
#include "NetworkUtils.hpp"

/*
Creates a socket using the specified
	domain (e.g., AF_INET for IPv4),
	type (e.g., SOCK_STREAM for TCP),
	and protocol (e.g., 0 for default protocol).

Returns the file descriptor of the created socket.
Throws an error if the socket creation fails.
*/
int	NetworkUtils::createSocket(int domain, int type, int protocol)
{
	// Attempt to create a socket with the specified parameters
	int	sockfd = socket(domain, type, protocol);

	// Check if the socket creation failed
	if (sockfd < 0)
		handleError("Failed to create socket"); // Throw an error if the operation fails

	// Return the created socket file descriptor
	return (sockfd);
}

/*
Sets a socket option for the given socket file descriptor.

Parameters:
	sockfd	- The socket file descriptor.
	level	- The protocol level at which the option resides.
	optname	- The name of the option.
	optval	- The value set for the option.

Throws an error if setting the option fails.

***
Example use case:

	NetworkUtils::setSocketOption(serverSock, SOL_SOCKET, SO_REUSEADDR, 1);

will call

	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

-> sockfd
	The socket file descriptor for which the option is being set.

-> `SOL_SOCKET`
	Applies the option to the socket layer itself (not protocol-specific).

-> `SO_REUSEADDR`
	Allowis immediate reuse of the address after the socket is closed.
	(The socket is allowed to bind to an address and port
	that was recently used by another socket)
	Useful to avoid errors like "address already in use"
	when restarting a server.

-> optval
	A pointer to the value of the option.
	Typically, this is set to 1 (true) to enable the option.
***
*/
void	NetworkUtils::setSocketOption(int sockfd, int level, int optname, int optval)
{
	// Set the socket option using setsockopt()
	if (setsockopt(sockfd, level, optname, &optval, sizeof(optval)) < 0)
	{
		// Throw an error if setting the option fails
		handleError("Failed to set socket option");
	}
}

/*
Binds a socket to a specific IP address and port.

Parameters:
	sockfd - The socket file descriptor.
	address - The IP address to bind to (in string format).
	port - The port number to bind to.

Throws an error if the address is invalid or the binding fails.
*/
void	NetworkUtils::bindSocket(int sockfd, const std::string &address, int port)
{
	struct sockaddr_in	addr;

	// Initialize the sockaddr_in structure to zero
	std::memset(&addr, 0, sizeof(addr));

	// Set the address family to AF_INET, indicating IPv4.
	addr.sin_family = AF_INET;

	// Convert the port number to network byte order
	addr.sin_port = htons(port);

	// Convert the IP address from its string form (e.g., "127.0.0.1") to its binary representation
	if (inet_pton(AF_INET, address.c_str(), &addr.sin_addr) <= 0)
	{
		// Throw an error if the IP address is invalid
		handleError("Invalid address");
	}

	// Bind the socket to the specified address and port
	if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		// Throw an error if binding fails
		handleError("Failed to bind socket");
	}
}

/*
Sets a socket to non-blocking mode.

Parameters:
	sockfd - The socket file descriptor.

Throws an error if setting non-blocking mode fails.

***
The O_NONBLOCK flag to enable non-blocking mode.
This ensures that oprations like recv() return immediately if
no data is available, instead of blocking indefinitely.
***
*/
void	NetworkUtils::setNonBlocking(int sockfd)
{
	// Retrieve the current socket flags
	int	flags = fcntl(sockfd, F_GETFL, 0);

	// Check if retrieving flags failed or setting new flags fails
	if (flags == -1 || fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		// Throw an error if non-blocking mode cannot be set
		handleError("Failed to set socket to non-blocking mode");
	}

	// // Check if retrieving the current flags failed.
	// if (flags == -1)
	// {
	// 	handleError("Failed to retrieve socket flags");
	// }

	// // Set the O_NONBLOCK flag to enable non-blocking mode.
	// if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1)
	// {
	// 	// Throw an error if non-blocking mode cannot be set
	// 	handleError("Failed to set socket to non-blocking mode");
	// }
}

/*
Marks the socket as a passive socket, which means it will listen for incoming connections.

Parameters:
	sockfd - The socket file descriptor.
	backlog - The maximum number of pending connections in the queue.

Throws an error if the listen call fails.
*/
void	NetworkUtils::listenSocket(int sockfd, int backlog)
{
	// Start listening for incoming connections
	if (listen(sockfd, backlog) < 0)
	{
		// Throw an error if the operation fails
		handleError("Failed to listen on socket");
	}
}

/*
Accepts a new incoming connection on a listening socket.

Parameters:
	sockfd - The socket file descriptor.
	clientAddr - A reference to a sockaddr_in struct to store the client's address.

Returns the file descriptor of the accepted connection.
Throws an error if accepting the connection fails.

***
accept(sockfd, (struct sockaddr*)&clientAddr, &addrLen);
params:
	sockfd: The listening socket file descriptor.
	struct sockaddr*)&clientAddr: Pointer to the client address structure.
	addrLen: Pointer to the size of the address structure.
***
*/
int	NetworkUtils::acceptConnection(int sockfd, struct sockaddr_in &clientAddr)
{
	socklen_t	addrLen = sizeof(clientAddr); // Size of the client address structure.

	// Accept a new connection and retrieve the client's socket file descriptor
	int	clientSock = accept(sockfd, (struct sockaddr*)&clientAddr, &addrLen);

	// Check if the accept operation failed
	if (clientSock < 0)
		handleError("Failed to accept connection"); // Throw an error if the operation fails

	// Return the client's socket file descriptor
	return (clientSock);
}

/*
Sends data through a connected socket.

Parameters:
	sockfd - The socket file descriptor.
	data - The string data to send.

Throws an error if the send operation fails.
*/
void	NetworkUtils::sendData(int sockfd, const std::string &data)
{
	// Attempt to send the data through the socket
	if (send(sockfd, data.c_str(), data.size(), 0) < 0)
	{
		// Throw an error if the send operation fails
		handleError("Failed to send data");
	}
}

/*
Receives data from a connected socket.

Parameters:
	sockfd - The socket file descriptor.

Returns the received data as a string.
Throws an error if the receive operation fails.

***
recv(sockfd, buffer, sizeof(buffer) - 1, 0);
params:
	sockfd: The socket file descriptor.
	buffer: The buffer where the received data will be stored.
	sizeof(buffer) - 1: The maximum number of bytes to read.
	0: Default behavior (blocking or non-blocking depends on socket settings).
***
*/
std::string	NetworkUtils::receiveData(int sockfd)
{
	char	buffer[1024]; // Buffer to hold incoming data
	std::memset(buffer, 0, sizeof(buffer)); // Clear the buffer before use.

	// Attempt to receive data from the socket
	int	bytesReceived = recv(sockfd, buffer, sizeof(buffer) - 1, 0);

	// Check if the receive operation failed
	if (bytesReceived < 0)
		handleError("Failed to receive data");

	// Return the received data as a string, limited to the number of bytes actually read.
	return (std::string(buffer, bytesReceived));
}

/*
Resolves a hostname to its corresponding IP address.

Parameters:
	hostname - The hostname to resolve.
Returns the resolved IP address as a string.

Throws an error if the hostname cannot be resolved.
*/
std::string	NetworkUtils::resolveHostName(const std::string &hostname)
{
	// Use gethostbyname to resolve the hostname
	struct hostent	*host = gethostbyname(hostname.c_str());

	// Check if the resolution failed
	if (!host || !host->h_addr_list[0])
	{
		// Throw an error if the hostname cannot be resolved
		handleError("Failed to resolve hostname");
	}

	// Return the resolved IP address
	return (inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));
}

/*
Closes a socket file descriptor.

Parameters:
	sockfd - The socket file descriptor.

Throws an error if closing the socket fails.
*/
void	NetworkUtils::closeSocket(int sockfd)
{
	// Attempt to close the socket
	if (close(sockfd) < 0)
	{
		// Throw an error if the close operation fails
		handleError("Failed to close socket");
	}
}

/*
Handles errors by throwing a runtime exception with an error message.

Parameters:
	msg - The error message to include in the exception.
*/
void	NetworkUtils::handleError(const std::string &msg)
{
	// Throw a runtime exception with the error message and the system error string
	throw (std::runtime_error(msg + ": " + std::strerror(errno)));
}
