/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Validator.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 23:04:14 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/05 13:42:10 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATOR_HPP
# define VALIDATOR_HPP

# include "ReplyTypes.hpp" // for numeric reply enum

# include <map>
# include <set>
# include <string>

/*
Validate semantic and syntax
*/
class Validator
{
	public:
		Validator(void);
		~Validator(void);
		
		bool	isValidCommand(const std::map<std::string, std::string> &command) const;
		bool	validateCommand(const std::map<std::string, std::string>& command) const;
		
		bool	isValidNickname(const std::string &nickname) const;
		bool	isValidChannelName(const std::string& channel) const;
		
		ReplyType	getError(void) const;
		const std::vector<std::string>&	getErrorArgs(void) const;
	
	private:
		bool	_validateJoinCommand(const std::map<std::string, std::string>& command) const;
		bool	_validatePrivmsgCommand(const std::map<std::string, std::string>& command) const;
		bool	_validatePassCommand(const std::map<std::string, std::string>& command) const;
		bool	_validateNickCommand(const std::map<std::string, std::string>& command) const;
		// ...

		bool	_setError(ReplyType error, const std::string &arg1 = "", const std::string &arg2 = "");
		mutable ReplyType	_error; // store the last error code
		mutable std::vector<std::string>	_errorArgs; // store arguments for the error reply
	
		static const size_t	MAX_NICKNAME_LENGTH;
		static const size_t	MAX_CHANNEL_NAME_LENGTH;
		
		static const std::set<std::string>	_validCommands;
};

#endif // VALIDATOR_HPP

/*
Commands to Implement

Authentication Commands:

PASS: Client provides the connection password.
NICK: Client sets their nickname.
USER: Client sets their username.

Channel Management Commands:

JOIN: Join a specific channel.
PART: Leave a specific channel.
TOPIC: View or change the channel's topic.
MODE: Change channel modes (i, t, k, o, l).
KICK: Eject a client from a channel.
INVITE: Invite a client to a channel.

Messaging Commands:

PRIVMSG: Send a private message to a user or channel.
NOTICE: Send a notice to a user or channel.
*/
