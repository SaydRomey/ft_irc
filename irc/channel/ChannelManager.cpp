#include "ChannelManager.hpp"
#include <iostream>
#include <vector>

ChannelManager::ChannelManager(Server& server) :_server(server)
{
}

ChannelManager::~ChannelManager()
{
}

void ChannelManager::joinManager(User &sender, const Message &msg)
{
	std::vector<std::pair<std::string, std::string> > ChannelsAndKeys = msg.getChannelsAndKeys();
	for (size_t i = 0; i < ChannelsAndKeys.size(); ++i)
	{
		const std::string &channelName = ChannelsAndKeys[i].first;
		const std::string &key = ChannelsAndKeys[i].second;
		if (_channels.find(channelName) == _channels.end())
		{
			std::cout << "not found channelname" << std::endl;

			// channel doesnt exist, making a new one
			Channel newChannel(channelName, sender);
			_channels[channelName] = newChannel;
			std::cout << "apres ajout channel" << std::endl;
		}
		else
		{
			std::cout << "adding member!" << std::endl;
			_channels[channelName].addMember(sender, key);
		}
	}
}


void ChannelManager::partManager(User &sender, const Message &msg)
{
	std::vector<std::string>  channelsVec = tokenize(msg.getParamsVec()[0], ',');
	const std::string &reason = msg.getTrailing();
	for (size_t i = 0; i < channelsVec.size(); ++i)
	{
		const std::string &channelName = channelsVec[i];
		if (_channels.find(channelName) == _channels.end())
		{
			sender.pendingPush(reply(ERR_NOSUCHCHANNEL, sender.getNickname(), channelName));
			continue;
		}
		_channels[channelName].removeMember(sender, reason);
		if (_channels[channelName].getMembers().empty()) //if channel empty, erase channel
			_channels.erase(channelName);
	}
}

void ChannelManager::inviteManager(User &sender, const Message &msg)
{
	std::string inviteeNickname = msg.getParamsVec()[0];
	std::vector<std::string> channelsVec = tokenize(msg.getParamsVec()[1], ',');
	for (size_t i = 0; i < channelsVec.size(); ++i)
	{
		std::string channelName = channelsVec[i];
		if (_channels.find(channelName) == _channels.end())
		{
			sender.pendingPush(reply(ERR_NOSUCHCHANNEL, sender.getNickname(), channelName));
			continue;
		}
		User* target = _server.getUserByNickname(inviteeNickname);
		if (target == NULL)
		{
			sender.pendingPush(reply(ERR_NOSUCHNICK, sender.getNickname(), inviteeNickname));
			continue;
		}
		_channels[channelName].invite(*target, sender);
	}
}

void ChannelManager::kickManager(User &sender, const Message &msg)
{
	std::string channelName = msg.getParamsVec()[0];
	std::vector<std::string> targetsNickname = tokenize(msg.getParamsVec()[1], ',');
	const std::string &reason = msg.getTrailing();

	if (_channels.find(channelName) == _channels.end())
	{
		sender.pendingPush(reply(ERR_NOSUCHCHANNEL, sender.getNickname(), channelName));
		return ;
	}
	for (size_t i = 0; i < targetsNickname.size(); ++i)
	{
		User* target = _server.getUserByNickname(targetsNickname[i]);
		if (!target)
		{
			sender.pendingPush(reply(ERR_NOSUCHNICK, sender.getNickname(), targetsNickname[i]));
			continue ;
		}
		_channels[channelName].kick(*target, sender, reason);
	}
}

void ChannelManager::modeManager(User &sender, const Message &msg)
{
	const std::string& channelName = msg.getParamsVec()[0];
	const std::string&	pswd = msg.getModeKey();
	const std::string&	limit = msg.getModeLimit();
	const std::string&	nickname = msg.getModeNick();
	User* target = _server.getUserByNickname(nickname);

	if (_channels.find(channelName) == _channels.end())
	{
		sender.pendingPush(reply(ERR_NOSUCHCHANNEL, sender.getNickname(), channelName));
		return ;
	}
	std::cout << nickname << "et ";
	if (target)
		std::cout << "target not null" << std::endl;
	else
		std::cout << "target is null" << std::endl;
	if (target == NULL && !nickname.empty())
	{
		sender.pendingPush(reply(ERR_NOSUCHNICK, sender.getNickname(), nickname));
		return ;
	}
	if (msg.getParamsVec().size() > 1)
	{
		const std::string&	modes = msg.getParamsVec()[1];
		_channels[channelName].setMode(modes, sender, pswd, limit, target, msg.getParams());
	}
	else
		_channels[channelName].getModes(sender);
}

void ChannelManager::topicManager(User &sender, const Message &msg)
{
	const std::string& channelName = msg.getParamsVec()[0];
	const std::string&	newTopic = msg.getTrailing();

	if (_channels.find(channelName) == _channels.end())
	{
		sender.pendingPush(reply(ERR_NOSUCHCHANNEL, sender.getNickname(), channelName));
		return ;
	}
	if (newTopic.empty())
		_channels[channelName].getTopic(sender);
	else if (!msg.getInput().empty() && msg.getInput()[msg.getInput().size() - 1] == ':')
		_channels[channelName].setTopic(sender, "");
	else
		_channels[channelName].setTopic(sender, newTopic);
}

void ChannelManager::quitManager(User &sender)
{
	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end();)
	{
		if (it->second.getMembers().find(&sender) != it->second.getMembers().end())
			it->second.removeMember(sender, "Disconnected");

		if (it->second.getMembers().empty())
		{
			std::map<std::string, Channel>::iterator toErase = it++;
			_channels.erase(toErase);
		}
		else
			++it;
	}
}

void ChannelManager::privmsgManager(User &sender, const std::string &channelName, const std::string &message)
{
	if (_channels.find(channelName) == _channels.end())
	{
		sender.pendingPush(reply(ERR_NOSUCHCHANNEL, sender.getNickname(), channelName));
		return ;
	}
	_channels[channelName].broadcast(sender, message, false);
}

