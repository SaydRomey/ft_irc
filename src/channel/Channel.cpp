#include "Channel.hpp"

Channel::Channel(std::string name, User& op) : _name(name), _topic(""), _password(""), _memberLimit(0)
{
	_modes['i'] = false;
	_modes['t'] = false;
	_modes['k'] = false;
	_modes['o'] = false;
	_modes['l'] = false;
	_members[&op]=true;
}
Channel::~Channel()
{

}

bool	Channel::addMember(User& user, std::string pswIfNeeded)
{
	if (_modes['i'] == true) //ERR_INVITEONLYCHAN
	{
		std::cout << ":server 473 " << user.getNickname() << " " << this->_name << " :Cannot join channel (+i)" << std::endl;
		return false;
	}
	if (_modes['k'] == true && _password.compare(pswIfNeeded) != 0) //ERR_BADCHANNELKEY
	{
		std::cout << ":server 475 " << user.getNickname() << " " << this->_name << " :Cannot join channel (+k)" << std::endl;
		return false;
	}
	if (_modes['l'] == true && _members.size() >= _memberLimit) //ERR_CHANNELISFULL
	{
		std::cout << ":server 471 " << user.getNickname() << " " << this->_name << " :Cannot join channel (+l)" << std::endl;
		return false;
	}
	_members[&user]=false;
	if (_members.find(&user) != _members.end())
		std::cout << ":" << user.getNickname() << "!user@host JOIN " << this->_name << std::endl;
	return true;
}

bool	Channel::removeMember(User& user, const std::string& reason = "") //voir pour garder bool et retourner un si on doit supp le channel car vide
{
	if (_members.find(&user) == _members.end()) //ERR_NOTONCHANNEL
	{
		std::cout << ":server 442 " << user.getNickname() << " " << this->_name << " :You're not on that channel" << std::endl;
		return false;
	}
	_members.erase(&user);
	if (_members.find(&user) == _members.end())
	{
		std::cout << ":" << user.getNickname() << "!user@localhost PART " << this->_name;
		if (!reason.empty())
			std::cout << " :" << reason;
		std::cout << std::endl;
	}
	return true;
	// les operateurs peuvent quitté, un channel peut etre sans operateur
}

std::map<User*,bool> Channel::getMembers()
{
	return _members;
}

bool	Channel::setTopic(User& user, const std::string& topic) //voir ce que weechat envoie si c'Est string vide
{
	//par defaut sur false tout le monde peut modifier le topic.
	//si mode +t donc true est activé, c'est seulement les op qui peuvent le changer
	if (_members.find(&user) == _members.end()) //ERR_NOTONCHANNEL
	{
		std::cout << ":server 442 " << user.getNickname() << " " << this->_name << " :You're not on that channel" << std::endl;
		return false;
	}
	if (_modes['t'] == true)
	{
		if (_members[&user] != true) //ERR_CHANOPRIVSNEEDED
		{
			std::cout << ":server 482 " << user.getNickname()<< " " << this->_name << " :You're not channel operator" << std::endl;
			return false;
		}
		_topic = topic;
		std::cout << ":" << user.getNickname() << "!user@host TOPIC " << _name << " :" << _topic << std::endl;
	}
	else
	{
		_topic = topic;
		std::cout << ":" << user.getNickname() << "!user@host TOPIC " << _name << " :" << _topic << std::endl;
	}
	return true;
}

bool Channel::getTopic(User& user)
{
	if (_members.find(&user) == _members.end()) //ERR_NOTONCHANNEL
	{
		std::cout << ":server 442 " << user.getNickname() << " " << this->_name << " :You're not on that channel" << std::endl;
		return false;
	}
	if (this->_topic.empty()) //ERR_NOTOPIC
		std::cout << ":server 331 " << _name << " :No topic is set" << std::endl;
	else
		std::cout << ":" << user.getNickname() << "!user@host TOPIC " << _name << " :" << _topic << std::endl;
	return true;
}

bool	Channel::kick(User &user, User& op, std::string reason)
{
	if (_members.find(&op) == _members.end()) //ERR_NOTONCHANNEL
	{
		std::cout << ":server 442 " << op.getNickname() << " " << this->_name << " :You're not on that channel" << std::endl;
		return false;
	}
	if (_members[&op] != true) //ERR_CHANOPRIVSNEEDED
	{
		std::cout << ":server 482 " << op.getNickname()<< " " << this->_name << " :You're not channel operator" << std::endl;
		return false;
	}
	if (_members.find(&user) == _members.end()) //ERR_USERNOTINCHANNEL
	{
		std::cout << ":server 441 " << op.getNickname() << " " << user.getNickname() << " " << this->_name << " :They aren't on that channel" << std::endl;
		return false;
	}
	_members.erase(&user);
	if (_members.find(&user) == _members.end())
		std::cout << ":server KICK " << this->_name << " " << user.getNickname() << " :" << reason << std::endl;
	return true;
}

bool	Channel::invite(User &user, User& op)
{
	if (_members.find(&op) == _members.end()) //ERR_NOTONCHANNEL
	{
		std::cout << ":server 442 " << op.getNickname() << " " << this->_name << " :You're not on that channel" << std::endl;
		return false;
	}
	if (_modes['i'] == true && _members[&op] != true) //ERR_CHANOPRIVSNEEDED
	{
		std::cout << ":server 482 " << op.getNickname()<< " " << this->_name << " :You're not channel operator" << std::endl;
		return false;
	}
	if (_members.find(&user) != _members.end()) //ERR_USERONCHANNEL
	{
		std::cout << ":server 443 " << op.getNickname() << " " << user.getNickname() << " " << this->_name << " :is already on channel" << std::endl;
		return false;
	}
	if (_modes['l'] == true && _members.size() >= _memberLimit) //ERR_CHANNELISFULL
	{
		std::cout << ":server 471 " << user.getNickname() << " " << this->_name << " :Cannot join channel (+l)" << std::endl;
		return false;
	}
	_members[&user]=false;
	if (_members.find(&user) != _members.end())
		std::cout << ":server 341 " << user.getNickname() << " " << this->_name << " :" << op.getNickname() << std::endl;
	return true;
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

bool Channel::setMode(std::string mode, User& op, const std::string& pwd, const std::string& limit, User* user)
{
	const std::string validMod = "itkol";
	if (_members.find(&op) == _members.end())
	{ // ERR_NOTONCHANNEL
		std::cout << ":server 442 " << op.getNickname() << " " << this->_name << " :You're not on that channel" << std::endl;
		return false;
	}
	if (_members[&op] != true)
	{ // ERR_CHANOPRIVSNEEDED
		std::cout << ":server 482 " << op.getNickname() << " " << this->_name << " :You're not channel operator" << std::endl;
		return false;
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
				std::cout << ":server 501 " << op.getNickname() << " :Syntax error in parameters" << std::endl;
				return false;
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
			else if (mode[i] == 'o')
			{
				if (user != NULL)
					addOperator(user, enable ? '+' : '-');
			}
			else if (mode[i] == 'l')
			{
				if (enable && isValidNb(limit))
					_memberLimit = std::atoi(limit.c_str());
				else if (!enable)
					_memberLimit = 0;
			}
		}
		else //Numéro d'erreur : 472, Message : ERR_UNKNOWNMODE, Format : "<mode char> :is an unknown mode" but not return
		{
			std::cout << ":server 472 " << op.getNickname() << " " << mode[i] << " :is an unknown mode" << std::endl;
			return false;
		}
	}
	return true;
}


bool	Channel::addOperator(User *user, const char addOrRemove)
{
	if (addOrRemove == '+')
	{
		this->_members[user] = true;
		return true;
	}
	else if (addOrRemove == '-')
	{
		this->_members[user] = false;
		return true;
	}
	return false;
}

void Channel::printMembers()
{
	std::cout << "Members list: " << std::endl;
	for (ItMembers it = this->_members.begin(); it != this->_members.end(); it++)
		std::cout << "-> " << it->first->getNickname() << " : " << it->second << std::endl;
}

void	Channel::printMode()
{
	std::cout << "Modes list: " << std::endl;
	for (ItModes it = this->_modes.begin(); it != this->_modes.end(); it++)
		std::cout << "-> " << it->first << " : " << it->second << std::endl;
}