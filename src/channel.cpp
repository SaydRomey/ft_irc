#include "channel.hpp"

Channel::Channel(std::string name, User& op) : _name(name), _topic("No topic"), _password(""), _memberLimit(0)
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
	if (_modes['i'] == false || (_modes['i'] == true && _password.compare(pswIfNeeded) == 0))
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
}

bool	Channel::removeMember(User& user)
{
	if (_members.find(&user) != _members.end())
		_members.erase(&user);
	if (_members.find(&user) == _members.end())
	{
		std::cout << "User " << user.getNickname() << " has left the channel " << this->_name << std::endl;
		return true;
	}
	return false;
	// voir si c'est pas un operator, sinon voir pour interdire soit remplacer
}

bool	Channel::setTopic(const std::string &topic, const User& op)
{
	if (_modes['t'] == true)
	{
		std::cout << "Channel " << this->_name << " theme: " << this->_topic << std::endl;
		return true;
	}
	User current = op;
	if (_members.find(&current) != _members.end() && _members[&current] == true)
	{
		_topic = topic;
		return true;
	}
	std::cout << "Unauthorized access: " << current.getNickname() << " is not an operator" << std::endl;
	return false;
}

bool	Channel::kick(User &user, const User& op, std::string reason)
{
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

bool	Channel::setMode(std::string mode, const User& op, std::string pswOrLimit = "")
{
	User current = op;
	if (_members.find(&current) != _members.end() && _members[&current] == true)
	{
		//next step: reste juste ca a faire, voir pour detecté si 1er caractere est - ou +
		//voir comment faire pour parcourir et faire en evitant trop de if else
	}
}

bool	Channel::addOperator(User &user, const char addOrRemove)
{
	if (addOrRemove == '+')
	{
		this->_members[&user] = true;
		return true;
	}
	else if (addOrRemove == '-')
	{
		this->_members[&user] = false;
		return true;
	}
	return false;
}
