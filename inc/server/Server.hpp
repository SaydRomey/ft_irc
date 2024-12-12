#ifndef SERVER_HPP
# define SERVER_HPP
# include "User.hpp"
# include "ChannelManager.hpp"
# include "Message.hpp"
# include <string>
# include <stdexcept>
# include <vector>
# include <map>
# include <poll.h>

typedef std::map<int, User> t_clientMap;
typedef std::vector<pollfd> t_pollfdVect;

class Server
{
public:
	Server(const std::string& port, const std::string& password);

	void run();

private:
	int			_port;
	std::string	_password;
	time_t		_time;
	bool		_isRunning;

	t_pollfdVect					_pollFds;
	t_clientMap						_clients;
	std::map<std::string, int>		_nickMap;
	ChannelManager					_chanManager;

	Reply	_rplGenerator;

	void	_acceptConnection();
	void	_messageRoundabout(User& client, const Message& msg);

	void	nick_cmd(User& client, const std::string& nick);
};

#endif
