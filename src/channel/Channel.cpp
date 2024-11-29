// #include "Channel.hpp"

// Channel::Channel(std::string name, User& op) : _name(name), _topic("No topic"), _password(""), _memberLimit(0)
// {
// 	_modes['i'] = false;
// 	_modes['t'] = true;
// 	_modes['k'] = false;
// 	_modes['o'] = false;
// 	_modes['l'] = false;
// 	_members[&op]=true;
// }
// Channel::~Channel()
// {

// }

// bool	Channel::addMember(User& user, std::string pswIfNeeded)
// {
// 	if (_modes['i'] == false || (_modes['i'] == true && _password.compare(pswIfNeeded) == 0)) // if not invite-only or (invite only, but argument matches _password)
// 	{
// 		if (_modes['l'] == false || _members.size() < _memberLimit) // if channel is not full
// 		{
// 			_members[&user]=false; // user in function param is not admin
// 			if (_members.find(&user) != _members.end()) // if user in function param exists in _members map
// 			{
// 				std::cout << user.getNickname() << " has joined the channel " << this->_name << "! :)" << std::endl;
// 				return true;
// 			}
// 			// if user does not exist in _members map
// 			std::cout << "tmp error message.." << std::endl; // ** tmp to avoid compiler error
// 			return (false);
// 		}
// 		else
// 		{
// 			std::cout << "The channel member limit has been reached" << std::endl;
// 			return false;
// 		}
// 	}
// 	else
// 	{
// 		std::cout << "Unauthorized access: Channel " << this->_name << " password invalid" << std::endl;
// 		return false;
// 	}
// }

// bool	Channel::removeMember(User& user)
// {
// 	if (_members.find(&user) != _members.end())
// 		_members.erase(&user);
// 	if (_members.find(&user) == _members.end())
// 	{
// 		std::cout << "User " << user.getNickname() << " has left the channel " << this->_name << std::endl;
// 		return true;
// 	}
// 	return false;
// 	// voir si c'est pas un operator, sinon voir pour interdire soit remplacer
// }

// bool	Channel::setTopic(const std::string &topic, const User& op)
// {
// 	if (_modes['t'] == true)
// 	{
// 		std::cout << "Channel " << this->_name << " theme: " << this->_topic << std::endl;
// 		return true;
// 	}
// 	User current = op;
// 	if (_members.find(&current) != _members.end() && _members[&current] == true)
// 	{
// 		_topic = topic;
// 		return true;
// 	}
// 	std::cout << "Unauthorized access: " << current.getNickname() << " is not an operator" << std::endl;
// 	return false;
// }

// bool	Channel::kick(User &user, const User& op, std::string reason)
// {
// 	User current = op;
// 	if (_members.find(&current) != _members.end() && _members[&current] == true)
// 	{
// 		this->removeMember(user);
// 		std::cout << "User " << user.getNickname() << " has been kicked for the following reason: " << reason << std::endl;
// 		return true;
// 	}
// 	std::cout << "Unauthorized access: " << current.getNickname() << " is not an operator" << std::endl;
// 	return false;
// }

// bool	Channel::invite(User &user, const User& op)
// {
// 	User current = op;
// 	if (_members.find(&current) != _members.end() && _members[&current] == true)
// 	{
// 		if (_modes['l'] == false || _members.size() < _memberLimit)
// 		{
// 			_members[&user]=false;
// 			if (_members.find(&user) != _members.end())
// 			{
// 				std::cout << user.getNickname() << " has been invited to join the channel " << this->_name << "! :)" << std::endl;
// 				return true;
// 			}
// 		}
// 		else
// 		{
// 			std::cout << "The channel member limit has been reached" << std::endl;
// 			return false;
// 		}
// 	}
// 	std::cout << "Unauthorized access: " << current.getNickname() << " is not an operator" << std::endl;
// 	return false;
// }

// // bool	isValidNb(const std::string& str)
// bool	isValidNb(std::string& str)
// {
// 	for (size_t i = 0; i < str.length(); i++)
// 	{
// 		if (!std::isdigit(str[i]))
// 			return false;
// 	}
// 	if (str.empty())
// 		return false;
// 	return true;
// }

// bool	Channel::setMode(std::string mode, const User& op, std::string pswOrLimit, User* user)
// {
// 	User current = op;
// 	const std::string validMod = "itkol";
// 	if (_members.find(&current) != _members.end() && _members[&current] == true)
// 	{
// 		if (mode[0] == '+')
// 		{
// 			for (size_t i = 1; i < mode.size(); i++)
// 			{
// 				if (validMod.find(mode[i]) != std::string::npos)
// 				{
// 					this->_modes[mode[i]] = true;
// 					if (mode[i] == 'k')
// 						if (!pswOrLimit.empty())
// 							this->_password = pswOrLimit;
// 					if (mode[i] == 'o')
// 						if (user != NULL)
// 							addOperator(user, '+');
// 					if (mode[i] == 'l')
// 						if (isValidNb(pswOrLimit) != false)
// 							this->_memberLimit = std::atoi(pswOrLimit.c_str());
// 				}
// 				else
// 				{
// 					//Numéro d'erreur : 472, Message : ERR_UNKNOWNMODE, Format : "<mode char> :is an unknown mode" but not return
// 					std::cout << "'" << mode[i] << "'" << " :is an unknown mode" << std::endl;
// 				}
// 			}
// 		}
// 		else if (mode[0] == '-')
// 		{
// 			for (size_t i = 1; i < mode.size(); i++)
// 			{
// 				if (validMod.find(mode[i]) != std::string::npos)
// 				{
// 					this->_modes[mode[i]] = false;
// 					if (mode[i] == 'k')
// 						this->_password = "";
// 					if (mode[i] == 'o')
// 						if (user != NULL)
// 							addOperator(user, '-');
// 					if (mode[i] == 'l')
// 						_memberLimit = 0;
// 				}
// 				else
// 				{
// 					//Numéro d'erreur : 472, Message : ERR_UNKNOWNMODE, Format : "<mode char> :is an unknown mode" but not return
// 					std::cout << "'" << mode[i] << "'" << " :is an unknown mode" << std::endl;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			std::cout << "Syntax error: a mode change must be preceded by a '-' or a '+'" << std::endl;
// 		}
// 	}
// }

// bool	Channel::addOperator(User *user, const char addOrRemove)
// {
// 	if (addOrRemove == '+')
// 	{
// 		this->_members[user] = true;
// 		return true;
// 	}
// 	else if (addOrRemove == '-')
// 	{
// 		this->_members[user] = false;
// 		return true;
// 	}
// 	return false;
// }
