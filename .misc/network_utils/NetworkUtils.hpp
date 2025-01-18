
#ifndef NETWORKUTILS_HPP
# define NETWORKUTILS_HPP

# include <string>			// std::string
// # include <vector>
# include <stdexcept>		// std::runtime_error
# include <sys/socket.h>	// socket(), bind(), listen(), accept(), send(), recv(), setsockopt()
# include <netinet/in.h>	// sockaddr_in, htons(), htonl(), ntohs(), ntohl()
# include <arpa/inet.h>		// inet_pton(), inet_ntoa()
# include <unistd.h>		// close()
# include <fcntl.h>			// fcntl()
# include <poll.h>			// poll()
# include <netdb.h>			// gethostbyname()
# include <cstring>			// std::memset()

/*
Contains static utility functions to simplify
common networking tasks when working with socket

Features:
	- Socket creation and configuration (e.g., createSocket(), setSocketOption()).
	- Managing connections (e.g., bindSocket(), listenSocket(), acceptConnection()).
	- Data transmission (e.g., sendData(), receiveData()).
	- Network-related utilities (e.g., resolveHostName()).
	- Error handling with descriptive runtime exceptions.

Designed to abstract low-level networking details,
to allow us to focus on higher-level logic.
*/
class NetworkUtils
{
	public:
		static int	createSocket(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
		static void	setSocketOption(int sockfd, int level, int optname, int optval);
		static void	bindSocket(int sockfd, const std::string& address, int port);
		static void	setNonBlocking(int sockfd);
		static void	listenSocket(int sockfd, int backlog = 5);
		static int	acceptConnection(int sockfd, struct sockaddr_in &clientAddr);
		static void	sendData(int sockfd, const std::string &data);
		static std::string	receiveData(int sockfd);
		static std::string	resolveHostName(const std::string &hostname);
		static void	closeSocket(int sockfd);
		static void	handleError(const std::string &msg);
};

#endif // NETWORKUTILS_HPP
