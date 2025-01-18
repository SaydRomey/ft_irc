#include "ChannelManager.hpp"
#include <iostream>
#include <vector> //

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
			// Channel inexistant donc creation du channel
			Channel newChannel(channelName, sender);
			_channels[channelName] = newChannel;
		}
		else
			_channels[channelName].addMember(sender, key);
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
		if (_channels[channelName].getMembers().empty()) //si mon channel n'a plus de membre supprime le channel
			_channels.erase(channelName);
	}
}

void ChannelManager::inviteManager(User &sender, const Message &msg)
{
	// Extraire les paramètres : channel et utilisateur à inviter
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
		// Inviter l'utilisateur
		User* target = _server.getUserByNickname(inviteeNickname);
		if (target == NULL) //ERR_NOSUCHNICK
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
	if (target == NULL && !nickname.empty()) //ERR_NOSUCHNICK
	{
		sender.pendingPush(reply(ERR_NOSUCHNICK, sender.getNickname(), nickname));
		return ;
	}
	if (msg.getParamsVec().size() > 1)
	{
		const std::string&	modes = msg.getParamsVec()[1];
		_channels[channelName].setMode(modes, sender, pswd, limit, target);
	}
	else
		_channels[channelName].getModes();
	
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
	else if (newTopic.compare(":") == 0)
		_channels[channelName].setTopic(sender, "");
	else
		_channels[channelName].setTopic(sender, newTopic);
}

void ChannelManager::quitManager(User &sender)
{
	//boucle dans tout les channels pour chercher si le sender est dedans car deconnexion = part des channels.
	//voir pour mettre un message PART different genre "disconnected", avec RPL_QUIT ou autre ou ajouté parametre à removeMembers
	//voir pour creer dans channel fonction quit sinon et faire un message different

	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end();)
	{
		if (it->second.getMembers().find(&sender) != it->second.getMembers().end())
			it->second.removeMember(sender, "Disconnected");

		if (it->second.getMembers().empty())
		{
			// increment iterator before erasing the current element
			std::map<std::string, Channel>::iterator toErase = it++;
			_channels.erase(toErase);
		}
		else
			++it;
	}
}

// sending messages to a channel
void ChannelManager::privmsgManager(User &sender, const std::string &channelName, const std::string &message)
{
	if (_channels.find(channelName) == _channels.end())
	{
		sender.pendingPush(reply(ERR_NOSUCHCHANNEL, sender.getNickname(), channelName));
		return ;
	}
	_channels[channelName].broadcast(sender, message);
}


// for direct channel replies
// void ChannelManager::privmsgManager(User &sender, const std::string &chan, const std::string &reply)
// {
// 	// if (_channels.count(chan) == 0)
// 	// 	return sender.pendingPush("INSERT REPLY 403 HERE");

// 	if (_channels.find(chan) == _channels.end())
// 	{
// 		sender.pendingPush(::reply(ERR_NOSUCHCHANNEL, sender.getNickname(), chan)); // "::" used to avoid conflict with function param "&reply"
// 		return ;
// 	}

// 	// const std::map<User*,bool> members(_channels[chan].getMembers());
// 	const std::map<User*, bool>& members = _channels[chan].getMembers();
// 	for (std::map<User*, bool>::const_iterator it = members.begin(); it != members.end(); ++it)
// 	{
// 		if (it->first != &sender)
// 			it->first->pendingPush(reply);
// 	}
// }
