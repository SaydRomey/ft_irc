/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 23:04:14 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/05 19:10:08 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATOR_HPP
# define VALIDATOR_HPP

# include "ReplyTypes.hpp" // for numeric reply enum

# include <map>
# include <set>
# include <string>
# include <sstream>
# include <vector>

/*	TOCHECK:

change isValidCommand to isValidSyntax ?
change validateCommand to isValidSemantic ?

*(does not handle chanop privileges related issues..)

*/

class Validator
{
	public:
		Validator(void);
		~Validator(void);
		
		bool	isValidCommand(const std::map<std::string, std::string> &command) const;
		bool	validateCommand(const std::map<std::string, std::string> &command) const;
		
		bool	isValidNickname(const std::string &nickname) const;
		bool	isValidChannelName(const std::string& channel) const;
		// ... other syntax validation ?
		
		ReplyType						getError(void) const;
		const std::vector<std::string>&	getErrorArgs(void) const;
	
	private:
		bool	_validatePassCommand(const std::map<std::string, std::string> &command) const;
		bool	_validateNickCommand(const std::map<std::string, std::string> &command) const;
		bool	_validateUserCommand(const std::map<std::string, std::string> &command) const;
		bool	_validateJoinCommand(const std::map<std::string, std::string> &command) const;
		bool	_validatePartCommand(const std::map<std::string, std::string> &command) const;
		bool	_validateTopicCommand(const std::map<std::string, std::string> &command) const;
		bool	_validateModeCommand(const std::map<std::string, std::string> &command) const;
		bool	_validateKickCommand(const std::map<std::string, std::string> &command) const;
		bool	_validateInviteCommand(const std::map<std::string, std::string> &command) const;
		bool	_validatePrivmsgCommand(const std::map<std::string, std::string> &command) const;
		bool	_validateNoticeCommand(const std::map<std::string, std::string> &command) const;

		mutable ReplyType					_error; // store the last error code
		mutable std::vector<std::string>	_errorArgs; // store arguments for the error reply
		
		bool	_setError(ReplyType error, const std::string &arg1 = "", const std::string &arg2 = "") const;
		bool	_noError(void) const;
		
		static const size_t	MAX_NICKNAME_LENGTH;
		static const size_t	MAX_CHANNEL_NAME_LENGTH;
		
		static const std::set<std::string>	_validCommands;
};

#endif // VALIDATOR_HPP
