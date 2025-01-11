#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include "User.hpp"
# include <vector>
# include <map>
# include "Reply.hpp"

typedef std::map<User*, bool> MapUser;
typedef std::map<char, bool> MapMode;
typedef MapUser::iterator ItMembers;
typedef MapMode::iterator ItModes;

class Channel
{
	private:
		std::string 			_name;
		std::string 			_topic;
		std::map<User*, bool>	_members; //true if it's an operator/admin
		std::map<char, bool>	_modes; //+ pour ajouté ou - pour enlever, peut enchainer les modes a suivre (ex: +itk)
		std::string 			_password;
		size_t					_memberLimit;

	public:
		Channel();
		Channel(const std::string& name, User& op);
		Channel& operator=(const Channel& other);
		~Channel();

		void						addMember(User& user, std::string pswIfNeeded = "");
		void						removeMember(User& user, const std::string& reason = "");
		void						setTopic(User& user, const std::string& topic);
		void						getTopic(User& user);
		void						kick(User &user, User& op, std::string reason);
		void						invite(User &user, User& op);
		// bool	isValidNb(const std::string& str);
		void						setMode(std::string mode, User& op, const std::string& pwd, const std::string& limit, User* user);
		void						addOperator(User *user, const char addOrRemove);

		void						printMembers();
		std::string 				membersList();
		void						printMode();
		//ajouter un getmember et getmode (car MODE sans parametres retourne 
		//les mode activés "Mode du channel xx activé : +i +o etc") 
		//mais si c'Est un op ca montrerait aussi genre les données types limite, pwd etc..
		const MapUser&				getMembers(void) const;

		// 
		void						broadcast(User& sender, const std::string& message); //
		// 
};

#endif