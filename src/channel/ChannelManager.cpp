#include "ChannelManager.hpp"

ChannelManager::ChannelManager()
{
}

ChannelManager::~ChannelManager()
{
}

void ChannelManager::join(User &sender, const Message &msg)
{
}

void ChannelManager::part(User &sender, const Message &msg)
{
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
	std::string chan = msg.getParams();
	if (_channels.count(chan) != 0)
	{
		//voir avec ced si il a fait une difference entre string vide et pas de string pour topic
		// if (msg.getTrail() is null)
		// 	_channels[chan].getTopic(sender);
		// else if (msg.getTrail() is empty)
		// 	_channels[chan].setTopic(sender, "");
		// else
		_channels[chan].setTopic(sender, msg.getTrailing());
	}
}

void ChannelManager::privmsg(User &sender, const std::string &chan, const std::string &reply)
{
	if (_channels.count(chan) == 0)
		return sender.pendingPush("INSERT REPLY 403 HERE");

	std::vector<User*>& chanMembers(_channels[chan].getMembers());
	for (std::vector<User*>::iterator it=chanMembers.begin(); it != chanMembers.end(); it++)
	{
		if (*it != &sender)
		(*it)->pendingPush(reply);
	}
}
