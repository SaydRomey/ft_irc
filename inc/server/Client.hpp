#ifndef CLIENT_HPP
# define CLIENT_HPP
#include <string>

namespace ft
{

class Client
{
public:
	Client(void); // Only for map element init, sets fd to -1
	Client(int fd);
	~Client();
	Client& operator=(const Client& that);

	void	setNickname(const std::string& nickname);
	void	setUsername(const std::string& username);
	void	setRegistered(void); // Throws logic_error if nickname or username isn't set

	const std::string&	getNickname(void) const;
	const std::string&	getUsername(void) const;
	bool				isRegistered(void) const;

	// TODO: move the following methods from Server into Client
	void	sendMessage(const std::string& msg);

private:
	int			_fd;
	std::string	_username;
	std::string	_nickname;
	bool		_registered;
};

}

#endif