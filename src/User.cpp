#include "User.hpp"

User::User(void): _fd(-1)
{}

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

void User::setFd(int fd)
{
	this->_fd = fd;
}

std::string User::getNickname() const
{
	return this->_nickname;
}

const std::string User::getUsername(void) const
{
	return this->_username;
}

void User::addToMsgBuffer(const std::string &packet)
{
	this->_msgBuffer += packet;
}

void User::resetMsgBuffer(const std::string &msg)
{
	this->_msgBuffer = msg;
}

const std::string& User::extractFromBuffer(void)
{
	size_t pos = _msgBuffer.find("\r\n");
	if (pos == std::string::npos)
		return "";
	pos += 2;
	std::string extract = _msgBuffer.substr(0, pos);
	_msgBuffer.erase(0, pos);
	return extract;
}

size_t User::pendingSize(void) const
{
	return _pending.size();
}

void User::pendingPush(const std::string& msg)
{
	_pending.push(msg);
}

const std::string& User::pendingPop(void)
{
	std::string msg = _pending.front();
	_pending.pop();
	return msg;
}
