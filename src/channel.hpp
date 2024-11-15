#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include "user.hpp"
# include <vector>

class Channel
{
	private:
		std::string _name;
		std::vector<User> _users;
	public:
		Channel(std::string name);
		~Channel();
};

#endif