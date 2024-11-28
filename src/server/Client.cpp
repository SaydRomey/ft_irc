#include "Client.hpp"

ft::Client::Client(void): _fd(-1), _username(), _nickname(), _registered(false), _authenticated(false)
{}

ft::Client::Client(int fd): _fd(fd), _username(), _nickname(), _registered(false), _authenticated(false)
{}

ft::Client::~Client()
{}

ft::Client &ft::Client::operator=(const Client &that)
{
	_fd = that._fd;
	_username = that._username;
	_nickname = that._nickname;
	_registered = that._registered;
	_authenticated = that._authenticated;
	return *this;
}

void ft::Client::setNickname(const std::string &nickname)
{
	_nickname = nickname;
}

void ft::Client::setUsername(const std::string &username)
{
	_username = username;
}

void ft::Client::setRegistered(void)
{
	if (_fd < 4 || _username == "" || _nickname == "" || _registered)
		throw std::logic_error("Registration not complete");
	_registered = true;
}

void ft::Client::setAuthenticated(bool auth)
{
	_authenticated = auth;
}

const std::string &ft::Client::getNickname(void) const
{
	return _nickname;
}

const std::string &ft::Client::getUsername(void) const
{
	return _username;
}

bool ft::Client::isRegistered(void) const
{
	return _registered;
}

bool ft::Client::isAuthenticated(void) const
{
	return _authenticated;
}

void ft::Client::sendMessage(const std::string &msg)
{
	send(_fd, msg.c_str(), msg.size(), 0);
}
