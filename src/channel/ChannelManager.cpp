#include "ChannelManager.hpp"

ChannelManager::ChannelManager()
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
			Channel newChannel(channelName, sender);
			_channels[channelName] = newChannel;
		}
		else
			_channels[channelName].addMember(sender, key);
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
	//voir avec ced si il a fait une difference entre string vide et pas de string pour topic
	// if (msg.getTrail() is null)
	// 	_channels[channelName].getTopic(sender);
	// else if (msg.getTrail() is empty)
	// 	_channels[channelName].setTopic(sender, "");
	// else
		_channels[channelName].setTopic(sender, msg.getTrailing());
}
