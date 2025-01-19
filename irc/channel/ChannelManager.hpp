#ifndef CHANNELMANAGER_HPP
# define CHANNELMANAGER_HPP

// # include <list> ?
# include <map> //
# include "Channel.hpp"
# include "Message.hpp"
# include "parsing_utils.hpp"
# include "Server.hpp"

// class ChannelManager;
// typedef void(ChannelManager::*t_chanFunc)(User&, const Message&);

class Server;

class ChannelManager
{
	private:
		std::map<std::string, Channel> _channels; //ERR_NOSUCHCHANNEL (403)
		Server&	_server;

		// void _initRoundabout(void);
	public:
		ChannelManager(Server& server);
		~ChannelManager();

		void joinManager(User& sender, const Message& msg); //voir pour tokenize avec parsing utils, tokenize
		void partManager(User& sender, const Message& msg);
		void privmsgManager(User& sender, const std::string &channelName, const Message& msg);
		void inviteManager(User& sender, const Message& msg);
		void kickManager(User& sender, const Message& msg);
		void modeManager(User& sender, const Message& msg);
		void topicManager(User& sender, const Message& msg);
		void quitManager(User& sender);

		void privmsgManager(User& sender, const std::string& chan, const std::string& reply);
		// std::map<std::string, t_chanFunc>	chanRoundabout;
};

#endif
