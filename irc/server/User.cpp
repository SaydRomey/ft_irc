#include "User.hpp"

User::User(void): _fd(-1), _username(), _nickname(), _perms(), _msgBuffer(), _pending()
{}

User::User(int fd): _fd(fd), _username(), _nickname("*"), _perms(), _msgBuffer(), _pending()
{}

User &User::operator=(const User &other)
{
	if (this != &other)
	{
		_fd = other._fd;
		_username = other._username;
		_nickname = other._nickname;
		_perms = other._perms;
		_msgBuffer = other._msgBuffer;
		_pending = other._pending;
	}
	return *this;
}

void User::setNickname(const std::string& nickname)
{
	this->_nickname = nickname;
	_perms |= PERM_NICK;
}

void User::setUsername(const std::string& username)
{
	this->_username = username;
	_perms |= PERM_USER;
}

void User::setFd(int fd)
{
	this->_fd = fd;
}

const std::string& User::getNickname() const
{
	return this->_nickname;
}

const std::string& User::getUsername(void) const
{
	return this->_username;
}

int User::getFd(void) const
{
	return _fd;
}

short User::getPerms(void) const
{
	return _perms;
}

void User::setPass(bool valid)
{
	std::cout << "User::setPass : " << (valid ? "valid" : "invalid") << std::endl;
	if (valid)
		_perms |= PERM_PASS;
	else
		_perms &= ~PERM_PASS;
}

void User::addToMsgBuffer(const std::string &packet)
{
	this->_msgBuffer += packet;
}

void User::resetMsgBuffer(const std::string &msg)
{
	this->_msgBuffer = msg;
}

const std::string User::extractFromBuffer(void)
{
	size_t pos = _msgBuffer.find("\r\n");
	if (pos == std::string::npos)
		return "";
	std::string extract = _msgBuffer.substr(0, pos);
	_msgBuffer.erase(0, pos + 2);
	return extract;
}

size_t User::pendingSize(void) const
{
	return _pending.size();
}

void User::pendingPush(const std::string& msg)
{
	std::cout << "[DEBUG] User::pendingPush called" << std::endl;
	_pending.push(msg);
}

const std::string User::pendingPop(void)
{
	std::string msg = _pending.front();
	_pending.pop();
	return msg;
}

bool User::addChannel(const std::string &chan)
{
	return _channels.insert(chan).second;
}

bool User::removeChannel(const std::string &chan)
{
	return _channels.erase(chan) > 0;
}

bool User::inChannel(const std::string &chan)
{
	return _channels.count(chan) > 0;
}
