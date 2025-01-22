#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include "User.hpp"
# include <vector>
# include <map>
# include "Reply.hpp"
# include <sstream>

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
		std::set<std::string>	_invitedList;

	public:
		Channel();
		Channel(const std::string& name, User& op);
		Channel& operator=(const Channel& other);
		~Channel();

		void						addMember(User& user, std::string pswIfNeeded = "");
		void						removeMember(User& user, const std::string& reason = "");
		void						setTopic(User& user, const std::string& topic);
		void						getTopic(User& user);
		void						kick(User &user, User& op, std::string reason = "");
		void						invite(User &user, User& op);
		void						setMode(std::string mode, User& op, const std::string& pswd, const std::string& limit, User* user, const std::string& params);
		void						addOperator(User *user, const char addOrRemove);

		std::string 				membersList();
		void						getModes(User &user);
		void						printMode();
		const MapUser&				getMembers(void) const;

		void						broadcast(User& sender, const std::string& message, bool include_sender);

};

#endif
