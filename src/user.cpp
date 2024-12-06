#include "user.hpp"

User::User(std::string username, std::string nickname) : _username(username), _nickname(nickname)
{

}

//User::User(int fd) : _fd(fd)
//{

//}

std::string User::getNickname() const //
{
	return this->_nickname;
}
