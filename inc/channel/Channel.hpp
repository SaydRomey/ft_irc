#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include "User.hpp"
# include <vector>
# include <map>

typedef std::map<User*, bool> MapUser;
typedef std::map<char, bool> MapMode;
typedef MapUser::iterator ItMembers;
typedef MapMode::iterator ItModes;

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
		bool	setTopic(User& user, std::string* topic = NULL);
		bool	kick(User &user, User& op, std::string reason);
		bool	invite(User &user, User& op);
		// bool	isValidNb(const std::string& str);
		bool	setMode(std::string mode, User& op, const std::string& pwd, const std::string& limit, User* user);
		bool	addOperator(User *user, const char addOrRemove);

		void	printMembers();
		void	printMode();
		//ajouter un getmember et getmode (car MODE sans parametres retourne les mode activés "Mode du channel xx activé : +i +o etc")
};

#endif