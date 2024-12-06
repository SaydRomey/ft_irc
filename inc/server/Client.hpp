#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <string>
# include <queue>
# include <stdexcept>
# include <sys/socket.h>

# define RESET		"\033[0m"
# define BOLD		"\033[1m"
# define ITALIC		"\033[3m"
# define UNDERLINE	"\033[4m"
# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define YELLOW		"\033[33m"
# define BLUE		"\033[34m"
# define PURPLE		"\033[35m"
# define CYAN		"\033[36m"
# define ORANGE		"\033[38;5;208m"
# define GRAYTALIC	"\033[3;90m"

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
	void	setAuthenticated(bool auth);

	const std::string&	getNickname(void) const;
	const std::string&	getUsername(void) const;
	bool				isRegistered(void) const;
	bool				isAuthenticated(void) const;

	// TODO: move the following methods from Server into Client
	void	sendMessage(const std::string& msg);

	// December 5th: new methods for User class (WIP)
	void	addToMsgBuffer(const std::string& packet);
	void	resetMsgBuffer(const std::string& msg);
	std::string extractFromBuffer();
	std::queue<std::string> pending;

private:
	int			_fd;
	std::string	_username;
	std::string	_nickname;
	bool		_registered;
	bool		_authenticated;

	// December 5th: new attributes for User class (WIP)
	std::string _msgBuffer;

};

}

#endif