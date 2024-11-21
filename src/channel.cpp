#include "channel.hpp"

Channel::Channel(std::string name, User& op) : _name(name), _topic("No topic"), _password(""), _memberLimit(0)
{
	_modes.insert_or_assign('i', false);
	_modes.insert_or_assign('t', false);
	_modes.insert_or_assign('k', false);
	_modes.insert_or_assign('o', false);
	_modes.insert_or_assign('l', false);
	_members[&op]=true;
}
Channel::~Channel()
{

}

bool	Channel::addMember(User& user)
{
	//voir si le mode i est pas activé sinon refus et mot de passe? 
	//voir aussi la limite d'utilisateur vs nombre actuel
	_members[&user]=false;
	if (_members.find(&user) != _members.end())
	{
		//ajouter message quand class user fini
		return true;
	}
	return false;
}

bool	Channel::removeMember(User& user)
{
	if (_members.find(&user) != _members.end())
		_members.erase(&user);
	if (_members.find(&user) == _members.end())
	{
		//ajouter message quand class user fini
		return true;
	}
	return false;
	// voir si c'est pas un operator, sinon voir pour interdire soit remplacer
}

bool	Channel::setTopic(const std::string &topic, const User& op)
{
	//voir si le mode t est bien en true sinon on affiche juste le topic
	User current = op;
	if (_members.find(&current) != _members.end() && _members[&current] == true)
	{
		_topic = topic;
		return true;
	}
	return false;
}

bool	Channel::kick(User &user, const User& op, std::string reason)
{
	User current = op;
	if (_members.find(&current) != _members.end() && _members[&current] == true)
	{
		
	}
}

bool	Channel::invite(User &user, const User& op)
{
	//voir la limite d'utilisateur vs nombre actuel
	User current = op;
	if (_members.find(&current) != _members.end() && _members[&current] == true)
	{
		
	}
}

bool	Channel::setMode(char mode, const User& op, std::string pswOrLimit = "")
{
	User current = op;
	if (_members.find(&current) != _members.end() && _members[&current] == true)
	{
		
	}
}

std::string Channel::getTopic()
{
	return this->_topic;
}
