#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include "user.hpp"
# include <vector>
# include <map>

class Channel
{
	private:
		std::string _name;
		std::string _topic;
		std::vector<User*> _members;
		std::vector<User*> _operators; //admin
		std::map<char, std::string> _modes;
		std::string _password;
		int _memberLimit;
	public:
		Channel(std::string name);
		~Channel();

		void	addMember(User *user);
		void	removeMember(User *user);
		void	setName(std::string name, User *operators);
		void	setTopic(const std::string &topic, User *user);
		std::string getTopic();

};

#endif