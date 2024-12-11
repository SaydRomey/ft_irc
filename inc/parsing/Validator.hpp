/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 23:04:14 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/06 20:41:29 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*	TOCHECK:

improve nickname validator
improve username validator
improve channel name validator
	(syntax rules, limits, etc) -> uniqueness handled in higher level..

*(does not handle chanop privileges related issues..)

*/

#ifndef VALIDATOR_HPP
# define VALIDATOR_HPP

# include "ReplyTypes.hpp" // for numeric reply enum

# include <map>
# include <string>
# include <vector>

/*	UNKNOWN at the end? or not at all, do we need PASS to start at 1 ?
*/
enum CommandType
{
	PASS = 0,
	NICK,
	USER,
	JOIN,
	PART,
	TOPIC,
	MODE,
	KICK,
	INVITE,
	PRIVMSG,
	NOTICE,
	CMD_UNKNOWN
};

class Validator
{
	public:		
		Validator(void);
		~Validator(void);
		
		bool	validateCommand(const std::map<std::string, std::string> &command) const;

		static const std::map<std::string, CommandType> &getCommandMap(void);
	
		
		ReplyType						getError(void) const;
		const std::vector<std::string>	&getErrorArgs(void) const;
		// CommandType						getCommandType(const std::string &cmd);
	
	private:
	
		// function pointer type for _validators
		typedef bool (Validator::*ValidatorFunc)(const std::map<std::string, std::string>&) const;

		// internal array mapping CommandType to ValidatorFunc
		static const ValidatorFunc	_validators[];
		
		static const std::map<std::string, CommandType>	_commandMap;
		// static const std::map<ReplyType, std::string>	_errorMessages;

		bool	_isValidNickname(const std::string &nickname) const;
		bool	_isValidChannelName(const std::string& channel) const;
		
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

		// error handling
		mutable ReplyType					_error; // store the last error code
		mutable std::vector<std::string>	_errorArgs; // store arguments for the error reply

		bool	_setError(ReplyType error, const std::string &arg1 = "", const std::string &arg2 = "") const;
		bool	_noError(void) const;

		bool	_validateCommandByType(CommandType cmdType, const std::map<std::string, std::string> &command) const;

		static const size_t			MAX_NICKNAME_LENGTH;
		static const size_t			MAX_CHANNEL_NAME_LENGTH;
		static const std::string	VALID_MODE_FLAGS;
};

#endif // VALIDATOR_HPP
