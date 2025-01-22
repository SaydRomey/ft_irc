#include "Channel.hpp"
#include <cstdlib> // atoi

Channel::Channel() : _name(""), _topic(""), _password(""), _memberLimit(0)
{
}

Channel::Channel(const std::string &name, User &op) : _name(name), _topic(""),
	_password(""), _memberLimit(0)
{
	_modes['i'] = false;
	_modes['t'] = false;
	_modes['k'] = false;
	_modes['o'] = false;
	_modes['l'] = false;
	_members[&op] = true;
	if (_members.find(&op) != _members.end())
	{
		this->broadcast(op, joinMsg(op.getNickname(), this->_name), true);
		if (this->_topic.empty())
			op.pendingPush(reply(RPL_NOTOPIC, op.getNickname(), this->_name));
		else
			op.pendingPush(reply(RPL_TOPIC, op.getNickname(), this->_name, this->_topic));
		op.pendingPush(reply(RPL_NAMEREPLY, op.getNickname(), this->_name, membersList()));
		op.pendingPush(reply(RPL_ENDOFNAMES, op.getNickname(), this->_name));
	}
}

Channel &Channel::operator=(const Channel &other)
{
	if (this == &other)
	{
		return (*this);
	}
	this->_name = other._name;
	this->_topic = other._topic;
	this->_members = other._members;
	this->_modes = other._modes;
	this->_password = other._password;
	this->_memberLimit = other._memberLimit;
	this->_invitedList = other._invitedList;
	return (*this);
}

Channel::~Channel()
{
}

// If a client’s JOIN command to the server is successful, the server MUST send,
	// in this order:

// 1.A JOIN message with the client as the message <source> and the channel they have joined as the first parameter of the message.
// 2.The channel’s topic (with RPL_TOPIC (332) and optionally RPL_TOPICWHOTIME (333)),
	// and no message if the channel does not have a topic.
// 3.A list of users currently joined to the channel (with one or more RPL_NAMREPLY (353) numerics followed by a single RPL_ENDOFNAMES (366) numeric). These RPL_NAMREPLY messages sent by the server MUST include the requesting client that has just joined the channel.
void Channel::addMember(User &user, std::string pswIfNeeded) // TODO: ajouté verif si user est deja dans le channel
{
	if (_modes['l'] == true && _members.size() >= _memberLimit)
		// ERR_CHANNELISFULL
	{
		user.pendingPush(reply(ERR_CHANNELISFULL, user.getNickname(), this->_name));
		return ;
	}
	if (_modes['k'] == true && _password.compare(pswIfNeeded) != 0)
		// ERR_BADCHANNELKEY
	{
		user.pendingPush(reply(ERR_BADCHANNELKEY, user.getNickname(), this->_name));
		return ;
	}
	if (_modes['i'] == true) // ERR_INVITEONLYCHAN
	{
		if (_invitedList.count(user.getNickname()) == 1)
			// user est dans la list des invités
			_invitedList.erase(user.getNickname());
				// supp car l'invitation aura servi
		else
		{
			user.pendingPush(reply(ERR_INVITEONLYCHAN, user.getNickname(), this->_name));
			return ;
		}
	}
	_members[&user] = false;
	if (_members.find(&user) != _members.end())
	{
		this->broadcast(user, joinMsg(user.getNickname(), this->_name), true);
		if (this->_topic.empty())
			user.pendingPush(reply(RPL_NOTOPIC, user.getNickname(), this->_name));
		else
			user.pendingPush(reply(RPL_TOPIC, user.getNickname(), this->_name, this->_topic));
		user.pendingPush(reply(RPL_NAMEREPLY, user.getNickname(), this->_name, membersList()));
		user.pendingPush(reply(RPL_ENDOFNAMES, user.getNickname(), this->_name));
	}
}

void Channel::removeMember(User &user, const std::string &reason)
{
	if (_members.find(&user) == _members.end()) // ERR_NOTONCHANNEL
	{
		user.pendingPush(reply(ERR_NOTONCHANNEL, user.getNickname(), this->_name));
		return ;
	}
	this->broadcast(user, partMsg(user.getNickname(), this->_name, reason), true);
	_members.erase(&user);
}

void Channel::setTopic(User &user, const std::string &topic)
{
	// par defaut sur false tout le monde peut modifier le topic.
	// si mode +t donc true est activé,
		// c'est seulement les op qui peuvent le changer
	if (_members.find(&user) == _members.end()) // ERR_NOTONCHANNEL
	{
		user.pendingPush(reply(ERR_NOTONCHANNEL, user.getNickname(), this->_name));
		return ;
	}
	if (_modes['t'] == true)
	{
		if (_members[&user] != true) // ERR_CHANOPRIVSNEEDED
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
				it->first->pendingPush(reply(RPL_TOPIC, user.getNickname(),
						this->_name, this->_topic));
			else
				it->first->pendingPush(reply(RPL_NOTOPIC, user.getNickname(), this->_name));
		}
	}
}

void Channel::getTopic(User &user)
{
	if (_members.find(&user) == _members.end()) // ERR_NOTONCHANNEL
	{
		user.pendingPush(reply(ERR_NOTONCHANNEL, user.getNickname(),
				this->_name));
		return ;
	}
	if (this->_topic.empty()) // RPL_NOTOPIC
		user.pendingPush(reply(RPL_NOTOPIC, user.getNickname(), this->_name));
	else
	{
		std::cout << "TOPIC-> " << this->_topic << std::endl;
		user.pendingPush(reply(RPL_TOPIC, user.getNickname(), this->_name, this->_topic));
	}
}

void Channel::kick(User &user, User &op, std::string reason)
{
	if (_members.find(&op) == _members.end()) // ERR_NOTONCHANNEL
	{
		op.pendingPush(reply(ERR_NOTONCHANNEL, op.getNickname(), this->_name));
		return ;
	}
	if (_members[&op] != true) // ERR_CHANOPRIVSNEEDED
	{
		op.pendingPush(reply(ERR_CHANOPRIVSNEEDED, op.getNickname(), this->_name));
		return ;
	}
	if (_members.find(&user) == _members.end()) // ERR_USERNOTINCHANNEL
	{
		op.pendingPush(reply(ERR_USERNOTINCHANNEL, op.getNickname(), user.getNickname(), this->_name));
		return ;
	}
	this->broadcast(op, kickMsg(op.getNickname(), this->_name, user.getNickname(), reason), true);
	_members.erase(&user);
}

void Channel::invite(User &user, User &op)
{
	if (_members.find(&op) == _members.end()) // ERR_NOTONCHANNEL
	{
		op.pendingPush(reply(ERR_NOTONCHANNEL, op.getNickname(), this->_name));
		return ;
	}
	if (_modes['i'] == true && _members[&op] != true) // ERR_CHANOPRIVSNEEDED
	{
		op.pendingPush(reply(ERR_CHANOPRIVSNEEDED, op.getNickname(), this->_name));
		return ;
	}
	if (_members.find(&user) != _members.end()) // ERR_USERONCHANNEL
	{
		op.pendingPush(reply(ERR_USERONCHANNEL, op.getNickname(), user.getNickname(), this->_name));
		return ;
	}
	if (_modes['l'] == true && _members.size() >= _memberLimit)
		// ERR_CHANNELISFULL
	{
		op.pendingPush(reply(ERR_CHANNELISFULL, op.getNickname(), this->_name));
		return ;
	}
	_invitedList.insert(user.getNickname());
	user.pendingPush(inviteMsg(op.getNickname(), user.getNickname(), this->_name));
	op.pendingPush(reply(RPL_INVITING, op.getNickname(), user.getNickname(), this->_name));
}

static bool	isValidNb(const std::string &str)
{
	for (size_t i = 1; i < str.length(); i++) // commencé a 1 pour ignoré le +-
	{
		if (!std::isdigit(str[i]))
			return (false);
	}
	if (str.empty())
		return (false);
	return (true);
}

void Channel::setMode(std::string mode, User &op, const std::string &pswd,
	const std::string &limit, User *user, const std::string& params)
{
	bool	enable;
	const std::string validMod = "itkol";
	if (_members.find(&op) == _members.end()) // ERR_NOTONCHANNEL
	{
		op.pendingPush(reply(ERR_NOTONCHANNEL, op.getNickname(), this->_name));
		return ;
	}
	if (_members[&op] != true) // ERR_CHANOPRIVSNEEDED
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
			if (currentSign == '\0') // ERR_UNKNOWNMODE
			{
				op.pendingPush(reply(ERR_UNKNOWNMODE, op.getNickname(), std::string(1, mode[i])));
				return ;
			}
			enable = (currentSign == '+');
			_modes[mode[i]] = enable;
			if (mode[i] == 'k')
			{
				if (enable && !pswd.empty())
					_password = pswd;
				else if (!enable)
					_password.clear();
			}
			else if (mode[i] == 'o')
			{
				if (_members.find(user) == _members.end())
				{
					op.pendingPush(reply(ERR_NOTONCHANNEL, user->getNickname(), this->_name));
					return ;
				}
				else
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
		else // ERR_UNKNOWNMODE
			op.pendingPush(reply(ERR_UNKNOWNMODE, op.getNickname(), std::string(1, mode[i])));
	}
	this->broadcast(op, setmodeMsg(op.getNickname(), params), true);
}

void Channel::addOperator(User *user, const char addOrRemove) //TODO ajouté le fait que ca renvoie la liste des operator a tous pour que ca se mettent a jour
{
	if (addOrRemove == '+')
		this->_members[user] = true;
	else if (addOrRemove == '-')
		this->_members[user] = false;
}

std::string Channel::membersList()
{
	std::string list;
	for (ItMembers it = this->_members.begin(); it != this->_members.end(); it++)
	{
		if (!list.empty())
			list += " ";
		if (it->second == true)
				list += "@";
		list += it->first->getNickname();
	}
	return (list);
}

void Channel::getModes(User &user)
{
	std::ostringstream strModes;
	std::ostringstream strParams; // separate stream for parameters
	std::string strEmpty = " ";

	// Add active modes to 'strModes'
	strModes << "+";
	for (ItModes it = this->_modes.begin(); it != this->_modes.end(); it++)
		if (it->second == true)
			strModes << it->first;
	
	// Check if the user is a member of the channel
	bool isMember = (_members.find(&user) != _members.end());

	// Add password and member limit parameters only for members 
	if (!_password.empty() && isMember)
	{
		strParams << " ";
		strParams << _password;
	}
	if (_memberLimit > 0 && isMember)
	{
		strParams << " ";
		strParams << _memberLimit;
	}

	// Send the reply to the user
	std::string modesWithParams = strModes.str() + strParams.str();
	if (modesWithParams.size() > 1) // Check if there are any active modes
		user.pendingPush(reply(RPL_CHANNELMODEIS, user.getNickname(), this->_name, modesWithParams));
	else
		user.pendingPush(reply(RPL_CHANNELMODEIS, user.getNickname(), this->_name, strEmpty));
	
	//fonctionne meme si la personne n'Est pas dans le channel.
	// pour tous: creer string avec la liste des modes actifs (+tik).
	// pour les membres seulement: apres la liste des modes actif, creer string avec les 
	// parametres si ya des modes actifs qui en ont (ex password ou memberlimit).
}

void Channel::printMode()
{
	std::cout << "Modes list: " << std::endl;
	for (ItModes it = this->_modes.begin(); it != this->_modes.end(); it++)
		std::cout << "-> " << it->first << " : " << it->second << std::endl;
}

const std::map<User *, bool> &Channel::getMembers(void) const
{
	return (_members);
}

// a revoir car apparement il y a plusieurs reply a ne pas enlevé le sender
void Channel::broadcast(User &sender, const std::string &message, bool include_sender)
{
	User	*member;

	for (ItMembers it = _members.begin(); it != _members.end(); ++it)
	{
		member = it->first;
		if (include_sender == true)
			member->pendingPush(message);
		else
		{
			if (member != &sender)
			{
				// Skip the sender
				member->pendingPush(message);
			}
		}
	}
}
