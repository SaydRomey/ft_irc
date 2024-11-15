#ifndef USER_HPP
# define USER_HPP

# include <iostream>

class User
{
	private:
		std::string _username;
		std::string _nickname;
		int fd;
	public:
		User(std::string username, std::string nickname, int fd);
		User(int fd);
};

#endif