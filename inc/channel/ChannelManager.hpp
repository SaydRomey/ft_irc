#ifndef CHANNELMANAGER_HPP
# define CHANNELMANAGER_HPP
# include <list>
# include "Channel.hpp"
# include "Message.hpp"
# include "parsing_utils.hpp"

// class ChannelManager;
// typedef void(ChannelManager::*t_chanFunc)(User&, const Message&);

class ChannelManager
{
	private:
		std::map<std::string, Channel> _channels; //ERR_NOSUCHCHANNEL (403)
		Reply& _reply;

		// void _initRoundabout(void);
	public:
		ChannelManager(Reply& reply);
		~ChannelManager();

		void join(User& sender, const Message& msg); //voir pour tokenize avec parsing utils, tokenize
		void part(User& sender, const Message& msg);
		void privmsg(User& sender, const Message& msg);
		void invite(User& sender, const Message& msg);
		void kick(User& sender, const Message& msg);
		void mode(User& sender, const Message& msg);
		void topic(User& sender, const Message& msg);

		void privmsg(User& sender, const std::string& chan, const std::string& reply);
		// std::map<std::string, t_chanFunc>	chanRoundabout;
};

#endif