#ifndef SERVER_HPP
# define SERVER_HPP
# include "Client.hpp"
# include "Parser.hpp"
# include <string>
# include <stdexcept>
# include <vector>
# include <map>
# include <poll.h>

typedef std::map<int, ft::Client> t_clientMap;
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

	t_pollfdVect	_pollFds;
	t_clientMap		_clients;
	Parser			parser;

	void	_acceptConnection();
	void	_messageRoundabout(const Message& msg);
};

#endif