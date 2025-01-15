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
	std::vector<std::pair<std::string, std::string> > ChannelsAndKeys = msg.getChannelsAndKeys();
	const std::string &reason = msg.getTrailing();
	for (size_t i = 0; i < ChannelsAndKeys.size(); ++i)
	{
		const std::string &channelName = ChannelsAndKeys[i].first;
		if (_channels.find(channelName) == _channels.end())
		{
			sender.pendingPush(reply(ERR_NOSUCHCHANNEL, sender.getNickname(), channelName));
			// std::cout << ":server 403 " << sender.getNickname() << " " << channelName << " :No such channel" << std::endl;
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
	std::vector<std::pair<std::string, std::string> > ChannelsAndKeys = msg.getChannelsAndKeys();
	for (size_t i = 0; i < ChannelsAndKeys.size(); ++i)
	{
		std::string channelName = ChannelsAndKeys[i].first;
		std::string inviteeNickname = ChannelsAndKeys[i].second;
		if (_channels.find(channelName) == _channels.end())
		{
			sender.pendingPush(reply(ERR_NOSUCHCHANNEL, sender.getNickname(), channelName));
			// std::cout << ":server 403 " << sender.getNickname() << " " << channelName << " :No such channel" << std::endl;
			continue;
		}
		// Inviter l'utilisateur
		// User& target = getUserByNickname(inviteeNickname); // Fonction à implémenter dans serveur?
		User* target = _server.getUserByNickname(inviteeNickname); //
		if (target == NULL) //ERR_NOSUCHNICK
		{
			sender.pendingPush(reply(ERR_NOSUCHNICK, sender.getNickname(), inviteeNickname));
			// std::cout << ":server 401 " << sender.getNickname() << " " << inviteeNickname << " :No such nick" << std::endl;
			continue;
		}
		_channels[channelName].invite(*target, sender); //
		// _channels[channelName].invite(target, sender);

		// Broadcast the invitation to the channel members
		// std::cout << ":" << sender.getNickname() << " INVITE " << inviteeNickname << " " << channelName << std::endl;
		// std::string	message = ":" + sender.getNickname() + " INVITE " + inviteeNickname + " " + channelName + "\r\n";
		// std::string	message = reply(RPL_INVITING, sender.getNickname(), inviteeNickname, channelName);
		// _channels[channelName].broadcast(sender, message); // already handled in Channel.invite() ?
	}
}

void ChannelManager::kickManager(User &sender, const Message &msg)
{
	(void)sender;
	(void)msg;
	//voir pour envoyé un message par default si pas de raison envoyé

	// const std::string&	channelName = 
	// const std::string&	targetNickname =
	// const std::string&	reason =

	// if (_channels.find(channelName) == _channels.end())
	// {
	// 	sender.pendingPush(reply(ERR_NOSUCHCHANNEL, sender.getNickname(), channelName));
	// 	return ;
	// }

	// // retrieve the target user
	// User*	target = _server.getUserByNickname(targetNickname);
	// if (!target)
	// {
	// 	sender.pendingPush(reply(ERR_NOSUCHNICK, sender.getNickname(), targetNickname));
	// 	return ;
	// }

	// _channels[channelName].kick(*target, sender, reason);
}

void ChannelManager::modeManager(User &sender, const Message &msg)
{
	(void)sender;
	(void)msg;
	// const std::string&	channelName = 
	// const std::string&	modes =
	// const std::string&	optionalParam =

	// if (_channels.find(channelName) == _channels.end())
	// {
	// 	sender.pendingPush(reply(ERR_NOSUCHCHANNEL, sender.getNickname(), channelName));
	// 	return ;
	// }

	// _channels[channelName].setMode(modes, sender, optionalParam, "", NULL);
	
}

void ChannelManager::topicManager(User &sender, const Message &msg)
{
	const std::string&	channelName = msg.getParams();
	const std::string&	newTopic = msg.getTrailing();

	if (_channels.find(channelName) == _channels.end())
	{
		sender.pendingPush(reply(ERR_NOSUCHCHANNEL, sender.getNickname(), channelName));
		// std::cout << ":server 403 " << sender.getNickname() << " " << channelName << " :No such channel" << std::endl;
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
	(void)sender;
	//boucle dans tout les channels pour chercher si le sender est dedans car deconnexion = part des channels.
	//voir pour mettre un message PART different genre "disconnected", avec RPL_QUIT ou autre ou ajouté parametre à removeMembers
	//voir pour creer dans channel fonction quit sinon et faire un message different

	// for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end();)
	// {
	// 	it->second.removeMember(sender, "Disconnected");
	// 	if (it->second.getMembers().empty())
	// 	{
	// 		it = _channels.erase(it);
	// 	}
	// 	else
	// 		++it;
	// }
}

// sending messages to a channel
void ChannelManager::privmsgManager(User &sender, const Message& msg)
{
	const std::string&	channelName = msg.getParams();
	const std::string&	message = msg.getTrailing();

	if (_channels.find(channelName) == _channels.end())
	{
		sender.pendingPush(reply(ERR_NOSUCHCHANNEL, sender.getNickname(), channelName));
		return ;
	}

	_channels[channelName].broadcast(sender, message); //?
}


// for direct channel replies
void ChannelManager::privmsgManager(User &sender, const std::string &chan, const std::string &reply)
{
	// if (_channels.count(chan) == 0)
	// 	return sender.pendingPush("INSERT REPLY 403 HERE");

	if (_channels.find(chan) == _channels.end())
	{
		sender.pendingPush(::reply(ERR_NOSUCHCHANNEL, sender.getNickname(), chan)); // "::" used to avoid conflict with function param "&reply"
		return ;
	}

	// const std::map<User*,bool> members(_channels[chan].getMembers());
	const std::map<User*, bool>& members = _channels[chan].getMembers();
	for (std::map<User*, bool>::const_iterator it = members.begin(); it != members.end(); ++it)
	{
		if (it->first != &sender)
			it->first->pendingPush(reply);
	}
}
