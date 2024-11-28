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
		std::map<User*, bool> _members; //true if it's an operator/admin
		std::map<char, bool> _modes; //+ pour ajouté ou - pour enlever, peut enchainer les modes a suivre (ex: +itk)
		std::string _password;
		size_t _memberLimit;
	public:
		Channel(std::string name, User& op);
		~Channel();

		bool	addMember(User& user, std::string pswIfNeeded = "");
		bool	removeMember(User& user);
		bool	setTopic(const std::string &topic, const User& op);
		bool	kick(User &user, const User& op, std::string reason);
		bool	invite(User &user, const User& op);
		bool	isValidNb(const std::string& str);
		bool	setMode(std::string mode, const User& op, std::string pswOrLimit = "", User* user = NULL);
		bool	addOperator(User *user, const char addOrRemove);
};

#endif