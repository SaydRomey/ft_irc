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
	//verifier aussi si k est activé car sinon rentre dedans
	if (_modes['i'] == false && _modes['k'] == false || (_modes['i'] == true && _password.compare(pswIfNeeded) == 0))
	{
		if (_modes['l'] == false || _members.size() < _memberLimit)
		{
			_members[&user]=false;
			if (_members.find(&user) != _members.end())
			{
				std::cout << user.getNickname() << " has joined the channel " << this->_name << "! :)" << std::endl;
				return true;
			}
		}
		else
		{
			std::cout << "The channel member limit has been reached" << std::endl;
			return false;
		}
	}
	else
	{
		std::cout << "Unauthorized access: Channel " << this->_name << " password invalid" << std::endl;
		return false;
	}
	return true;
}

bool	Channel::removeMember(User& user)
{
	//voir si plus personne dans le channel car si c'Est le cas on supprime channel
	if (_members.find(&user) != _members.end())
		_members.erase(&user);
	if (_members.find(&user) == _members.end())
	{
		std::cout << "User " << user.getNickname() << " has left the channel " << this->_name << std::endl;
		return true;
	}
	return false;
	// les operateurs peuvent quitté, un channel peut etre sans operateur
}

bool	Channel::setTopic(const User& user, std::string* topic)
{
	//par defaut sur false tout le monde peut modifier le topic.
	//si mode +t donc true est activé, c'est seulement les op qui peuvent le changer
	User _user = user;
	if (_members.find(&_user) != _members.end()) //etrange car la je verifie que le membre est dedans, alors que je devrais detecté si il est pas dedans mais quand j'inverse je rentre toujours dans cettet condition
	{
		std::cout << ":server 442 " << user.getNickname() << " " << this->_name << " :You're not on that channel" << std::endl;
		return false;
	}
	if (topic == NULL)
	{
		if (this->_topic.empty())
			std::cout << ":server 331 " << _name << " :No topic is set" << std::endl;
		else
			std::cout << ":" << user.getNickname() << "!user@host TOPIC " << _name << " :" << _topic << std::endl;
		return true;
	}
	if (_modes['t'] == true)
	{
		if (_members[&_user] != true)
		{
			std::cout << ":server 482 " << user.getNickname()<< " " << this->_name << " :You're not channel operator" << std::endl;
			return false;
		}
		_topic = *topic;
		std::cout << ":" << user.getNickname() << "!user@host TOPIC " << _name << " :" << _topic << std::endl;
	}
	else
	{
		_topic = *topic;
		std::cout << ":" << user.getNickname() << "!user@host TOPIC " << _name << " :" << _topic << std::endl;
	}
	return true;
}

bool	Channel::kick(User &user, const User& op, std::string reason)
{
	//enlever l'appel a removemember car sinon ca met 2 messages
	User current = op;
	if (_members.find(&current) != _members.end() && _members[&current] == true)
	{
		this->removeMember(user);
		std::cout << "User " << user.getNickname() << " has been kicked for the following reason: " << reason << std::endl;
		return true;
	}
	std::cout << "Unauthorized access: " << current.getNickname() << " is not an operator" << std::endl;
	return false;
}

bool	Channel::invite(User &user, const User& op)
{
	User current = op;
	if (_members.find(&current) != _members.end() && _members[&current] == true)
	{
		if (_modes['l'] == false || _members.size() < _memberLimit)
		{
			_members[&user]=false;
			if (_members.find(&user) != _members.end())
			{
				std::cout << user.getNickname() << " has been invited to join the channel " << this->_name << "! :)" << std::endl;
				return true;
			}
		}
		else
		{
			std::cout << "The channel member limit has been reached" << std::endl;
			return false;
		}
	}
	std::cout << "Unauthorized access: " << current.getNickname() << " is not an operator" << std::endl;
	return false;
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

bool	Channel::setMode(std::string mode, const User& op, const std::string& pswOrLimit, User* user)
{
	User current = op;
	const std::string validMod = "itkol";
	if (_members.find(&current) == _members.end())
	{
		std::cout << ":server 442 " << current.getNickname() << " " << this->_name << " :You're not on that channel" << std::endl;
		return false;
	}
	if (_members[&current] != true)
	{
		std::cout << ":server 482 " << current.getNickname()<< " " << this->_name << " :You're not channel operator" << std::endl;
		return false;
	}
	if (mode[0] == '+')
	{
		for (size_t i = 1; i < mode.size(); i++)
		{
			if (validMod.find(mode[i]) != std::string::npos)
			{
				this->_modes[mode[i]] = true;
				if (mode[i] == 'k')
					if (!pswOrLimit.empty())
						this->_password = pswOrLimit;
				if (mode[i] == 'o')
					if (user != NULL)
						addOperator(user, '+');
				if (mode[i] == 'l')
					if (isValidNb(pswOrLimit) != false)
						this->_memberLimit = std::atoi(pswOrLimit.c_str());
			}
			else
			{
				//Numéro d'erreur : 472, Message : ERR_UNKNOWNMODE, Format : "<mode char> :is an unknown mode" but not return
				std::cout << ":server 472 " << current.getNickname() << " " << mode[i] << " :is an unknown mode" << std::endl;
				return false;
			}
		}
	}
	else if (mode[0] == '-')
	{
		for (size_t i = 1; i < mode.size(); i++)
		{
			if (validMod.find(mode[i]) != std::string::npos)
			{
				this->_modes[mode[i]] = false;
				if (mode[i] == 'k')
					this->_password = "";
				if (mode[i] == 'o')
					if (user != NULL)
						addOperator(user, '-');
				if (mode[i] == 'l')
					_memberLimit = 0;
			}
			else
			{
				//Numéro d'erreur : 472, Message : ERR_UNKNOWNMODE, Format : "<mode char> :is an unknown mode" but not return
				std::cout << ":server 472 " << current.getNickname() << " " << mode[i] << " :is an unknown mode" << std::endl;
				return false;
			}
		}
	}
	else
	{
		std::cout << "Syntax error: a mode change must be preceded by a '-' or a '+'" << std::endl;
		return false;
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
	// for (std::map<User*, bool>::iterator it = this->_members.begin(); it != this->_members.end(); it++)
	// 	std::cout << "-> " << it->first->getNickname() << std::endl;
}

void	Channel::printMode()
{
	std::cout << "Modes list: " << std::endl;
	for (ItModes it = this->_modes.begin(); it != this->_modes.end(); it++)
		std::cout << "-> " << it->first << " : " << it->second << std::endl;
}