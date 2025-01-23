#ifndef CHANNELMANAGER_HPP
# define CHANNELMANAGER_HPP

# include <map>
# include "Channel.hpp"
# include "Message.hpp"
# include "utils.hpp"
# include "Server.hpp"

class Server;

class ChannelManager
{
	private:
		std::map<std::string, Channel> _channels;
		Server&	_server;

		// void _initRoundabout(void);
	public:
		ChannelManager(Server& server);
		~ChannelManager();

		void joinManager(User& sender, const Message& msg);
		void partManager(User& sender, const Message& msg);
		void privmsgManager(User& sender, const std::string &channelName, const Message& msg);
		void inviteManager(User& sender, const Message& msg);
		void kickManager(User& sender, const Message& msg);
		void modeManager(User& sender, const Message& msg);
		void topicManager(User& sender, const Message& msg);
		void quitManager(User& sender);

		void privmsgManager(User& sender, const std::string& chan, const std::string& reply);
};

#endif
