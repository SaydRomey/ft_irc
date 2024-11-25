#ifndef CLIENT_HPP
# define CLIENT_HPP
#include <string>

namespace ft
{

class Client
{
public:
	Client(int fd);
	~Client();
	Client& operator=(const Client& that);

	void	setNickname(const std::string& nickname);
	void	setUsername(const std::string& username);

	// TODO: Move Server::_sendMessage method into Client class
	void	sendMessage(const std::string& msg);

private:
	int			_fd;
	std::string	_username;
	std::string	_nickname;
	bool		_registered;
};

}

#endif