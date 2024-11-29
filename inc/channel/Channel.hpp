#ifndef CHANNEL_HPP
# define CHANNEL_HPP

// # include <iostream>
// # include "user.hpp"
// # include <vector>
// # include <map>

// class Channel
// {
// 	private:
// 		std::string _name;
// 		std::string _topic;
// 		std::map<User*, bool> _members; //true if it's an operator/admin
// 		std::map<char, bool> _modes; //+ pour ajouté ou - pour enlever, peut enchainer les modes a suivre (ex: +itk)
// 		std::string _password;
// 		size_t _memberLimit;
// 	public:
// 		Channel(std::string name, User& op);
// 		~Channel();

// 		bool	addMember(User& user, std::string pswIfNeeded = "");
// 		bool	removeMember(User& user);
// 		bool	setTopic(const std::string &topic, const User& op);
// 		bool	kick(User &user, const User& op, std::string reason);
// 		bool	invite(User &user, const User& op);
// 		bool	isValidNb(const std::string& str);
// 		bool	setMode(std::string mode, const User& op, std::string pswOrLimit = "", User* user = NULL);
// 		bool	addOperator(User *user, const char addOrRemove);
// };

// #endif

/* ************************************************************************** */

/*	**tmp** had to fix this to not have compil errors
	TODO: Remove this version when i understand the original one better *!!
*/
// bool	Channel::setMode(std::string mode, const User& op, std::string pswOrLimit, User* user)
// {
// 	User current = op;
// 	const std::string validMod = "itkol";

// 	// Check if the operator has privileges
// 	if (_members.find(&current) == _members.end() || !_members[&current])
// 	{
// 		std::cout << "Error: Operator privileges required." << std::endl;
// 		return (false);
// 	}

// 	// **tmp** Validate mode syntax (remove when parsing will handle this?) **tmp**
// 	if (mode.empty() || (mode[0] != '+' && mode[0] != '-'))
// 	{
// 		std::cout << "Syntax error: a mode change must be preceded by a '-' or a '+'." << std::endl;
// 		return (false);
// 	}

// 	bool	addingMode = (mode[0] == '+'); // to detect if we add or remove (to avoid two separate 'mode[0] == '+/-' loops?)

// 	// Process each mode character
// 	size_t i = 1;
// 	while (i < mode.size())
// 	{
// 		char currentMode = mode[i];
// 		if (validMod.find(currentMode) == std::string::npos)
// 		{
// 			std::cout << "'" << currentMode << "' :is an unknown mode." << std::endl;
// 			continue ; // Skip invalid modes
// 		}

// 		_modes[currentMode] = addingMode;

// 		// Handle specific modes
// 		if (currentMode == 'k') // Set or clear password
// 		{
// 			if (addingMode && !pswOrLimit.empty())
// 				_password = pswOrLimit;
// 			else if (!addingMode)
// 				_password.clear();
// 		}
// 		else if (currentMode == 'o') // Add or remove operator
// 		{
// 			if (user != NULL)
// 				addOperator(user, addingMode ? '+' : '-');
// 		}
// 		else if (currentMode == 'l') // Set or clear member limit
// 		{
// 			if (addingMode)
// 			{
// 				if (isValidNb(pswOrLimit))
// 					_memberLimit = std::atoi(pswOrLimit.c_str());
// 				else
// 					std::cout << "Error: Invalid member limit value." << std::endl;
// 			}
// 			else
// 			{
// 				_memberLimit = 0;
// 			}
// 		}
// 		++i;
// 	}

// 	return (true);
// }
