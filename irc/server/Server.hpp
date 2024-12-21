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

typedef std::map<int, User>			t_clientMap;
typedef std::vector<pollfd>			t_pfdVect;
typedef std::map<std::string, int>	t_strIntMap;

class Server
{
public:
	Server(const std::string& port, const std::string& password);
	void		run();
	static void	signalHandler(int signum);

	static t_strIntMap commandMap;

private:
	static bool	_isRunning;
	int			_port;
	std::string	_password;
	time_t		_time;

	t_pfdVect		_pollFds;
	t_clientMap		_clientMap;
	t_strIntMap		_nickMap;
	Reply			_rplGenerator;
	ChannelManager	_chanManager;

	int					_safePoll();
	void				_acceptConnection();
	t_pfdVect::iterator	_closeConnection(t_pfdVect::iterator& it);
	void	_messageRoundabout(User& client, const Message& msg);

	void	broadcast(const std::string& msg, int senderFd=-1);
	void	pass_cmd(User& client, const Message& msg);
	void	user_cmd(User& client, const Message& msg);
	void	nick_cmd(User& client, const Message& msg);
	void	privmsg_cmd(User& client, const Message& msg);
};

#endif