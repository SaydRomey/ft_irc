#include "User.hpp"

User::User(void) : _fd(0), _isRegistred(false)
{

}

User::User(std::string username, std::string nickname) : _username(username), _nickname(nickname), _fd(0), _isRegistred(false)
{

}

User::User(int fd) : _fd(fd), _isRegistred(false)
{

}

void User::setNickname(const std::string& nickname)
{
	if(!nickname.empty())
		this->_nickname = nickname;
	else
		std::cout << "The supplied string cannot be empty" << std::endl;
}

void User::setUsername(const std::string& username)
{
	if(!username.empty())
		this->_username = username;
	else
		std::cout << "The supplied string cannot be empty" << std::endl;
}

void User::setFd(const int& fd)
{
	this->_fd = fd;
}

void User::setRegistered(void)
{
	this->_isRegistred = true;
}

void setAuthentified(void)
{
	
}

std::string User::getNickname() const
{
	return this->_nickname;
}

const std::string User::getUsername(void) const
{
	return this->_username;
}

bool User::isRegistered(void) const
{
	return this->_isRegistred;
}

void User::sendMessage(const std::string& msg)
{

}
