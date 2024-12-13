#include "ChannelManager.hpp"

ChannelManager::ChannelManager(Reply& reply) : _reply(reply)
{

}

ChannelManager::~ChannelManager()
{
}

void ChannelManager::join(User &sender, const Message &msg)
{
	std::vector<std::pair<std::string, std::string> > ChannelsAndKeys = msg.getChannelsAndKeys();
	for (size_t i = 0; i < ChannelsAndKeys.size(); ++i)
	{
		const std::string &channelName = ChannelsAndKeys[i].first;
		const std::string &key = ChannelsAndKeys[i].second;
		if (_channels.find(channelName) == _channels.end())
		{
			// Channel inexistant donc creation du channel
			Channel newChannel(channelName, sender, this->_reply);
			_channels[channelName] = newChannel;
		}
		else
			_channels[channelName].addMember(sender, key, msg.getReply());
	}
}


void ChannelManager::part(User &sender, const Message &msg)
{
	std::vector<std::pair<std::string, std::string> > ChannelsAndKeys = msg.getChannelsAndKeys();
	const std::string &reason = msg.getTrailing();
	for (size_t i = 0; i < ChannelsAndKeys.size(); ++i)
	{
		const std::string &channelName = ChannelsAndKeys[i].first;
		if (_channels.find(channelName) == _channels.end())
		{
			std::cout << ":server 403 " << sender.getNickname() << " " << channelName << " :No such channel" << std::endl;
			continue;
		}
		_channels[channelName].removeMember(sender, reason);
		if (_channels[channelName].getMembers().empty()) //si mon channel n'a plus de membre supprime le channel
			_channels.erase(channelName);
	}
}

void ChannelManager::privmsg(User &sender, const Message &msg)
{
}

void ChannelManager::invite(User &sender, const Message &msg)
{
	// Extraire les paramètres : channel et utilisateur à inviter
	std::vector<std::pair<std::string, std::string>> ChannelsAndKeys = msg.getChannelsAndKeys();
	for (size_t i = 0; i < ChannelsAndKeys.size(); ++i)
	{
		std::string channelName = ChannelsAndKeys[i].first;
		std::string inviteeNickname = ChannelsAndKeys[i].second;
		if (_channels.find(channelName) == _channels.end())
		{
			std::cout << ":server 403 " << sender.getNickname() << " " << channelName << " :No such channel" << std::endl;
			continue;
		}
		// Inviter l'utilisateur
		User& target = getUserByNickname(inviteeNickname); // Fonction à implémenter dans serveur?
		if (target == NULL) //ERR_NOSUCHNICK
		{
			std::cout << ":server 401 " << sender.getNickname() << " " << inviteeNickname << " :No such nick" << std::endl;
			continue;
		}
		_channels[channelName].invite(target, sender);
		std::cout << ":" << sender.getNickname() << " INVITE " << inviteeNickname << " " << channelName << std::endl;
	}
}

void ChannelManager::kick(User &sender, const Message &msg)
{

}

void ChannelManager::mode(User &sender, const Message &msg)
{
	
}

void ChannelManager::topic(User &sender, const Message &msg)
{
	std::string channelName = msg.getParams();
	if (_channels.find(channelName) == _channels.end())
	{
		std::cout << ":server 403 " << sender.getNickname() << " " << channelName << " :No such channel" << std::endl;
		return ;
	}
	if (msg.getTrailing().empty())
		_channels[channelName].getTopic(sender);
	else if (msg.getTrailing().compare(":") == 0)
		_channels[channelName].setTopic(sender, "");
	else
		_channels[channelName].setTopic(sender, msg.getTrailing());
}

void ChannelManager::privmsg(User &sender, const std::string &chan, const std::string &reply)
{
	if (_channels.count(chan) == 0)
		return sender.pendingPush("INSERT REPLY 403 HERE");

	const std::map<User*,bool> members(_channels[chan].getMembers());
	for (std::map<User*,bool>::const_iterator it=members.begin(); it != members.end(); it++)
	{
		if (it->first != &sender)
			it->first->pendingPush(reply);
	}
}
