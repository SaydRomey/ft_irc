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
	typedef void(Server::*t_servFunc)(User&, const Message&);

private:
	int			_port;
	std::string	_password;
	time_t		_time;
	bool		_isRunning;

	t_pollfdVect						_pollFds;
	t_clientMap							_clients;
	std::map<std::string, int>			_nickMap;
	ChannelManager						_chanManager;
	std::map<std::string, t_servFunc>	_serverRoundabout;
	Reply								_rplGenerator;

	void	_initRoundabout();
	void	_acceptConnection();
	void	_messageRoundabout(User& client, const Message& msg);

	void	broadcast(const std::string& msg, int senderFd=-1);
	void	pass_cmd(User& client, const Message& msg);
	void	user_cmd(User& client, const Message& msg);
	void	nick_cmd(User& client, const Message& msg);
	void	privmsg_cmd(User& client, const Message& msg);
};

#endif