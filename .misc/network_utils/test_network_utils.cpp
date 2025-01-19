
#include "NetworkUtils.hpp"
#include <iostream>

/*
Sets up and starts the IRC server.

Parameters:
	port - The port number to listen on.
	password - The password required for client connections.
*/
void	startServer(int port, const std::string& password)
{
	// Create a socket
	int	serverSock = NetworkUtils::createSocket();

	// Set socket options to allow address reuse
	// This prevents "address already in use" errors during restart
	NetworkUtils::setSocketOption(serverSock, SOL_SOCKET, SO_REUSEADDR, 1);

	// Bind the socket to all available interfaces ("0.0.0.0") and the given port
	NetworkUtils::bindSocket(serverSock, "0.0.0.0", port);

	// Set the socket to non-blocking mode
	// This allows the server to perform asynchronous I/O operations
	NetworkUtils::setNonBlocking(serverSock);

	// Mark the socket as listening for incoming connections
	// The backlog parameter specifies the maximum number of queued connections
	NetworkUtils::listenSocket(serverSock, 10);

	// Server setup complete, print confirmation
	std::cout << "Server listening on port " << port << "\n";

	// Polling setup (simplified)
	// Create a pollfd structure to monitor the server socket for incoming connections
	struct pollfd	fds[1];
	fds[0].fd = serverSock; // Set the server socket file descriptor
	fds[0].events = POLLIN; // Monitor for incoming connection events

	// Main event loop
	while (true)
	{
		// Use poll() to monitor the server socket
		// -1 timeout means it will wait indefinitely for an event
		int	pollCount = poll(fds, 1, -1);

		// Handle polling errors
		if (pollCount < 0)
		{
			NetworkUtils::handleError("Polling error");
		}

		// Check if there is an incoming connection
		if (fds[0].revents & POLLIN)
		{
			// Accept the incoming connection
			struct sockaddr_in	clientAddr; // Client address structure
			int	clientSock = NetworkUtils::acceptConnection(serverSock, clientAddr);

			// Handle the new connection
			std::cout << "New connection accepted\n";

			// Send a welcome message to the client
			NetworkUtils::sendData(clientSock, "Welcome to the IRC server!\n");

			// Close the client socket after sending the welcome message
			// (will not reach here in this loop)
			NetworkUtils::closeSocket(clientSock);
		}
	}

	// Cleanup (not reached in this infinite loop, but kept for consistency)
	NetworkUtils::closeSocket(serverSock);
}

/*
Main entry point for the NetworkUtils test.
Validates input arguments and starts the server.
*/
void	test_network_utils(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return ;
	}

	int			port = std::atoi(argv[1]);
	std::string	password = argv[2];

	try
	{
		startServer(port, password);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return ;
	}
}
