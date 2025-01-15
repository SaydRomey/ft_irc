#include "Channel.hpp"
#include <cstdlib> // atoi

Channel::Channel() : _name(""), _topic(""), _password(""), _memberLimit(0)
{}

Channel::Channel(const std::string& name, User &op) :
	_name(name), _topic(""), _password(""), _memberLimit(0)
{
	_modes['i'] = false;
	_modes['t'] = false;
	_modes['k'] = false;
	_modes['o'] = false;
	_modes['l'] = false;
	_members[&op]=true;
}

Channel& Channel::operator=(const Channel& other) 
{
	if (this == &other) 
	{
		return *this;
	}
	this->_name = other._name;
	this->_topic = other._topic;
	this->_members = other._members;
	this->_modes = other._modes;
	this->_password = other._password;
	this->_memberLimit = other._memberLimit;
	this->_invitedList = other._invitedList;
	return *this;
}

Channel::~Channel()
{}

// If a client’s JOIN command to the server is successful, the server MUST send, in this order:

// 1.A JOIN message with the client as the message <source> and the channel they have joined as the first parameter of the message.
// 2.The channel’s topic (with RPL_TOPIC (332) and optionally RPL_TOPICWHOTIME (333)), and no message if the channel does not have a topic.
// 3.A list of users currently joined to the channel (with one or more RPL_NAMREPLY (353) numerics followed by a single RPL_ENDOFNAMES (366) numeric). These RPL_NAMREPLY messages sent by the server MUST include the requesting client that has just joined the channel.
void	Channel::addMember(User& user, std::string pswIfNeeded)
{
	if (_modes['l'] == true && _members.size() >= _memberLimit) //ERR_CHANNELISFULL
	{
		user.pendingPush(reply(ERR_CHANNELISFULL, user.getNickname(), this->_name));
		return ;
	}
	if (_modes['k'] == true && _password.compare(pswIfNeeded) != 0) //ERR_BADCHANNELKEY
	{
		user.pendingPush(reply(ERR_BADCHANNELKEY, user.getNickname(), this->_name));
		return ;
	}
	if (_modes['i'] == true) //ERR_INVITEONLYCHAN
	{
		if (_invitedList.count(user.getNickname()) == 0)//user est dans la list des invités
			_invitedList.erase(user.getNickname()); //supp car l'invitation aura servi
		else
		{
			user.pendingPush(reply(ERR_INVITEONLYCHAN, user.getNickname(), this->_name));
			return ;
		}
	}
	_members[&user]=false;
	if (_members.find(&user) != _members.end()) //RPL_JOIN
	{
		this->broadcast(user, reply(RPL_JOIN, user.getNickname(), this->_name));
		// for (ItMembers it = this->_members.begin(); it != this->_members.end(); it++)
		// 	it->first->pendingPush(reply(RPL_JOIN, user.getNickname(), this->_name));
		if (this->_topic.empty())
			user.pendingPush(reply(RPL_NOTOPIC, user.getNickname(), this->_name));
		else
			user.pendingPush(reply(RPL_TOPIC, user.getNickname(), this->_name, this->_topic));
		user.pendingPush(reply(RPL_NAMEREPLY, user.getNickname(), this->_name, membersList()));
		user.pendingPush(reply(RPL_ENDOFNAMES, user.getNickname(), this->_name));
	}
}

void	Channel::removeMember(User& user, const std::string& reason)
{
	if (_members.find(&user) == _members.end()) //ERR_NOTONCHANNEL
	{
		user.pendingPush(reply(ERR_NOTONCHANNEL, user.getNickname()));
		return ;
	}
	_members.erase(&user);
	if (_members.find(&user) == _members.end())
	{
		std::string rep = reply(RPL_PART, user.getNickname(), this->_name);
		if (!reason.empty())
		{
			rep += " :";
			rep += reason;
		}
		this->broadcast(user, rep);
	}
}

void	Channel::setTopic(User& user, const std::string& topic)
{
	//par defaut sur false tout le monde peut modifier le topic.
	//si mode +t donc true est activé, c'est seulement les op qui peuvent le changer
	if (_members.find(&user) == _members.end()) //ERR_NOTONCHANNEL
	{
		user.pendingPush(reply(ERR_NOTONCHANNEL, user.getNickname(), this->_name));
		return ;
	}
	if (_modes['t'] == true)
	{
		if (_members[&user] != true) //ERR_CHANOPRIVSNEEDED
		{
			user.pendingPush(reply(ERR_CHANOPRIVSNEEDED, user.getNickname(), this->_name));
			return ;
		}
		_topic = topic;
		for (ItMembers it = this->_members.begin(); it != this->_members.end(); it++)
		{
			if (!this->_topic.empty())
				it->first->pendingPush(reply(RPL_TOPIC, user.getNickname(), this->_name, this->_topic));
			else
				it->first->pendingPush(reply(RPL_NOTOPIC, user.getNickname(), this->_name));
		}
	}
	else
	{
		_topic = topic;
		for (ItMembers it = this->_members.begin(); it != this->_members.end(); it++)
		{
			if (!this->_topic.empty())
				it->first->pendingPush(reply(RPL_TOPIC, user.getNickname(), this->_name, this->_topic));
			else
				it->first->pendingPush(reply(RPL_NOTOPIC, user.getNickname(), this->_name));
		}
	}
}

void Channel::getTopic(User& user)
{
	if (_members.find(&user) == _members.end()) //ERR_NOTONCHANNEL
	{
		user.pendingPush(reply(ERR_NOTONCHANNEL, user.getNickname(), this->_name));
		return ;
	}
	if (this->_topic.empty()) //RPL_NOTOPIC
		user.pendingPush(reply(RPL_NOTOPIC, user.getNickname(), this->_name));
	else
		user.pendingPush(reply(RPL_TOPIC, user.getNickname(), this->_name, this->_topic));
}

void	Channel::kick(User &user, User& op, std::string reason)
{
	if (_members.find(&op) == _members.end()) //ERR_NOTONCHANNEL
	{
		op.pendingPush(reply(ERR_NOTONCHANNEL, op.getNickname(), this->_name));
		return ;
	}
	if (_members[&op] != true) //ERR_CHANOPRIVSNEEDED
	{
		op.pendingPush(reply(ERR_CHANOPRIVSNEEDED, op.getNickname(), this->_name));
		return ;
	}
	if (_members.find(&user) == _members.end()) //ERR_USERNOTINCHANNEL
	{
		op.pendingPush(reply(ERR_USERNOTINCHANNEL, user.getNickname(), this->_name));
		return ;
	}
	_members.erase(&user);
	if (_members.find(&user) == _members.end())
		this->broadcast(op, reply(RPL_KICK, user.getNickname(), this->_name, reason));
		// for (ItMembers it = this->_members.begin(); it != this->_members.end(); it++)
		// 	it->first->pendingPush(reply(RPL_KICK, user.getNickname(), this->_name, reason));
}

// void	Channel::invite(User &user, User& op)
// {
// 	if (_members.find(&op) == _members.end()) //ERR_NOTONCHANNEL
// 	{
// 		op.pendingPush(reply(ERR_NOTONCHANNEL, op.getNickname(), this->_name));
// 		return ;
// 	}
// 	if (_modes['i'] == true && _members[&op] != true) //ERR_CHANOPRIVSNEEDED
// 	{
// 		op.pendingPush(reply(ERR_CHANOPRIVSNEEDED, op.getNickname(), this->_name));
// 		return ;
// 	}
// 	if (_members.find(&user) != _members.end()) //ERR_USERONCHANNEL
// 	{
// 		op.pendingPush(reply(ERR_USERONCHANNEL, op.getNickname(), user.getNickname(), this->_name));
// 		return ;
// 	}
// 	if (_modes['l'] == true && _members.size() >= _memberLimit) //ERR_CHANNELISFULL
// 	{
// 		op.pendingPush(reply(ERR_CHANNELISFULL, op.getNickname(), this->_name));
// 		return ;
// 	}
// 	_members[&user]=false;
// 	if (_members.find(&user) != _members.end())
// 		for (ItMembers it = this->_members.begin(); it != this->_members.end(); it++)
// 			it->first->pendingPush(reply(RPL_INVITING, op.getNickname(), user.getNickname(), this->_name));
// }

void	Channel::invite(User &user, User& op)
{
	if (_members.find(&op) == _members.end()) //ERR_NOTONCHANNEL
	{
		op.pendingPush(reply(ERR_NOTONCHANNEL, op.getNickname(), this->_name));
		return ;
	}
	if (_modes['i'] == true && _members[&op] != true) //ERR_CHANOPRIVSNEEDED
	{
		op.pendingPush(reply(ERR_CHANOPRIVSNEEDED, op.getNickname(), this->_name));
		return ;
	}
	if (_members.find(&user) != _members.end()) //ERR_USERONCHANNEL
	{
		op.pendingPush(reply(ERR_USERONCHANNEL, op.getNickname(), user.getNickname(), this->_name));
		return ;
	}
	if (_modes['l'] == true && _members.size() >= _memberLimit) //ERR_CHANNELISFULL
	{
		op.pendingPush(reply(ERR_CHANNELISFULL, op.getNickname(), this->_name));
		return ;
	}
	_invitedList.insert(user.getNickname());
	this->broadcast(op, reply(RPL_INVITING, op.getNickname(), user.getNickname(), this->_name));
}

static bool	isValidNb(const std::string& str)
{
	for (size_t i = 1; i < str.length(); i++) //commencé a 1 pour ignoré le +-
	{
		if (!std::isdigit(str[i]))
			return false;
	}
	if (str.empty())
		return false;
	return true;
}

// bool	Channel::setMode(std::string mode, User& op, const std::string& pswOrLimit, User* user)
// {
// 	const std::string validMod = "itkol";
// 	if (_members.find(&op) == _members.end()) //ERR_NOTONCHANNEL
// 	{
// 		std::cout << ":server 442 " << op.getNickname() << " " << this->_name << " :You're not on that channel" << std::endl;
// 		return false;
// 	}
// 	if (_members[&op] != true) //ERR_CHANOPRIVSNEEDED
// 	{
// 		std::cout << ":server 482 " << op.getNickname()<< " " << this->_name << " :You're not channel operator" << std::endl;
// 		return false;
// 	}
// 	if (mode[0] == '+')
// 	{
// 		for (size_t i = 1; i < mode.size(); i++)
// 		{
// 			if (validMod.find(mode[i]) != std::string::npos)
// 			{
// 				this->_modes[mode[i]] = true;
// 				if (mode[i] == 'k')
// 					if (!pswOrLimit.empty())
// 						this->_password = pswOrLimit;
// 				if (mode[i] == 'o')
// 					if (user != NULL)
// 						addOperator(user, '+');
// 				if (mode[i] == 'l')
// 					if (isValidNb(pswOrLimit) != false)
// 						this->_memberLimit = std::atoi(pswOrLimit.c_str());
// 			}
// 			else
// 			{
// 				//Numéro d'erreur : 472, Message : ERR_UNKNOWNMODE, Format : "<mode char> :is an unknown mode" but not return
// 				std::cout << ":server 472 " << op.getNickname() << " " << mode[i] << " :is an unknown mode" << std::endl;
// 				return false;
// 			}
// 		}
// 	}
// 	else if (mode[0] == '-')
// 	{
// 		for (size_t i = 1; i < mode.size(); i++)
// 		{
// 			if (validMod.find(mode[i]) != std::string::npos)
// 			{
// 				this->_modes[mode[i]] = false;
// 				if (mode[i] == 'k')
// 					this->_password = "";
// 				if (mode[i] == 'o')
// 					if (user != NULL)
// 						addOperator(user, '-');
// 				if (mode[i] == 'l')
// 					_memberLimit = 0;
// 			}
// 			else
// 			{
// 				//Numéro d'erreur : 472, Message : ERR_UNKNOWNMODE, Format : "<mode char> :is an unknown mode" but not return
// 				std::cout << ":server 472 " << op.getNickname() << " " << mode[i] << " :is an unknown mode" << std::endl;
// 				return false;
// 			}
// 		}
// 	}
// 	else //ERR_UNKNOWNMODE 
// 	{
// 		std::cout << ":server 501 " << op.getNickname() << " :Syntax error in parameters" << std::endl;
// 		return false;
// 	}
// 	return true;
// }

void Channel::setMode(std::string mode, User& op, const std::string& pwd, const std::string& limit, User* user)
{
	const std::string validMod = "itkol";
	if (_members.find(&op) == _members.end())// ERR_NOTONCHANNEL
	{ 
		op.pendingPush(reply(ERR_NOTONCHANNEL, op.getNickname(), this->_name));
		return ;
	}
	if (_members[&op] != true)// ERR_CHANOPRIVSNEEDED
	{ 
		op.pendingPush(reply(ERR_CHANOPRIVSNEEDED, op.getNickname(), this->_name));
		return ;
	}
	char currentSign = '\0'; // Pour garder la trace de + ou -
	for (size_t i = 0; i < mode.size(); ++i)
	{
		if (mode[i] == '+' || mode[i] == '-')
			currentSign = mode[i];
		else if (validMod.find(mode[i]) != std::string::npos)
		{
			// Mode valide
			if (currentSign == '\0') //ERR_UNKNOWNMODE 
			{
				op.pendingPush(reply(ERR_UNKNOWNMODE, op.getNickname(), std::string(1, mode[i])));
				return ;
			}
			bool enable = (currentSign == '+');
			_modes[mode[i]] = enable;
			if (mode[i] == 'k')
			{
				if (enable && !pwd.empty())
					_password = pwd;
				else if (!enable)
					_password.clear();
			}
			else if (mode[i] == 'o') //TODO verifié si dans le channel et si il est pas deja op
			{
				if (user != NULL)
					addOperator(user, enable ? '+' : '-');
			}
			else if (mode[i] == 'l')
			{
				if (enable && isValidNb(limit))
					_memberLimit = atoi(limit.c_str());
				else if (!enable)
					_memberLimit = 0;
			}
		}
		else //ERR_UNKNOWNMODE
			op.pendingPush(reply(ERR_UNKNOWNMODE, op.getNickname(), std::string(1, mode[i])));
	}
	//:operateur!user@host MODE #channel +im -k
	//this->broadcast(op, reply(RPL_MODE, op.getNickname(), mode, this->_name)) ?
}


void	Channel::addOperator(User *user, const char addOrRemove)
{
	if (addOrRemove == '+')
		this->_members[user] = true;
	else if (addOrRemove == '-')
		this->_members[user] = false;
}

void Channel::printMembers()
{
	std::cout << "Members list: " << std::endl;
	for (ItMembers it = this->_members.begin(); it != this->_members.end(); it++)
		std::cout << "-> " << it->first->getNickname() << " : " << it->second << std::endl;
}

std::string Channel::membersList()
{
	std::string list;
	for (ItMembers it = this->_members.begin(); it != this->_members.end(); it++)
	{
		if (list.empty())
		{
			list = it->first->getNickname();
			continue ;
		}
		else
			list += " ";
		list += it->first->getNickname();
	}
	return list;
}

void	Channel::printMode()
{
	std::cout << "Modes list: " << std::endl;
	for (ItModes it = this->_modes.begin(); it != this->_modes.end(); it++)
		std::cout << "-> " << it->first << " : " << it->second << std::endl;
}

const std::map<User *, bool> &Channel::getMembers(void) const
{
	return _members;
}

//a revoir car apparement il y a plusieurs reply a ne pas enlevé le sender
void	Channel::broadcast(User& sender, const std::string& message)
{
	for (ItMembers it = _members.begin(); it != _members.end(); ++it)
	{
		User*	member = it->first;
		if (member != &sender)
		{
			// Skip the sender
			member->pendingPush(message);
		}
	}
}
