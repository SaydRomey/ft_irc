#include "User.hpp"

User::User(void): _fd(-1)
{}

User::User(const std::string& username, const std::string& nickname)
	: _username(username), _nickname(nickname), _fd(-1)
{}

User::User(int fd) : _fd(fd)
{}

User&	User::operator=(const User& other)
{
	if (this != &other) { // Avoid self-assignment
		_fd = other._fd;
		_nickname = other._nickname;
		_username = other._username;
		_msgBuffer = other._msgBuffer;
		_pending = other._pending;
	}
	return *this;
}
//

int	User::getFd() const
{
	return _fd;
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
		// return ""; // temporary object can't be returned as a reference ?
	{
		static const std::string	emptyString = ""; // tmp solution ?
		return (emptyString);
	}
	// 
	pos += 2;
	// std::string extract = _msgBuffer.substr(0, pos);
	// _msgBuffer.erase(0, pos);
	// _msgBuffer.insert(0, extract);
	// return extract;
	_extractedMessage = _msgBuffer.substr(0, pos);
	_msgBuffer.erase(0, pos);
	return _extractedMessage;
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
	// std::string msg = _pending.front();
	// _pending.pop();
	// return msg;
	_currentMessage = _pending.front();
	_pending.pop();
	return _currentMessage;
}
