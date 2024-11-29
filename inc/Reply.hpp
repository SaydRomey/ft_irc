/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 01:35:46 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/29 17:21:12 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_HPP
# define REPLY_HPP

# include <string>
# include <map>
# include <vector>

/*
constant or macro for the server name
*/
// const std::string SERVER_NAME = "ircserv";
# define SERVER_NAME "ircserv"

/*
when a client sends a command to a server, the server will send a reply
(except in a few special commands)

replies are used to
	acknowledge that a command was processed correctly,
	indicate an error or to
	provide information when the command performs a server query
	(e.g., asking for the list of users or channels)

usual characteristics:
	- always include a prefix
	- the command will be a 3 digit code
	(full list of possible replies in RFC2812)
	- the first param is always the target of the reply, typically a nick

examples of valid IRC replies:

:ircserv 001 [nickname] :Welcome to Fun Times City, dear [nickname]

:ircserv 433 * [nickname] :Nickname is already in use.

:ircserv 332 [nickname] #cmsc23300 :A channel for CMSC 23300 students


*/

class Reply
{
	public:
		Reply();
		~Reply();

		std::string	generateReply(const std::string &key, const std::vector<std::string> &args);
		
		// General replies
		std::string	welcome(const std::string &userNickname);
		std::string	authenticationFailed(const std::string &reason);
		std::string	joinSuccess(const std::string &channelName);
		std::string	privateMessage(const std::string &sender, const std::string &receiver, const std::string &message);
		std::string	channelMessage(const std::string &sender, const std::string &channel, const std::string &message);
		std::string	kickMessage(const std::string &operatorName, const std::string &target, const std::string &channel, const std::string &reason);
		std::string	inviteMessage(const std::string &operatorName, const std::string &target, const std::string &channel);
		std::string	topicMessage(const std::string &channel, const std::string &topic);
		std::string	modeChange(const std::string &channel, const std::string &mode, const std::string &parameter);
	
		// Error replies
		std::string	needMoreParams(const std::string &command);
		std::string	alreadyRegistered(const std::string &userNickname);
		std::string	nicknameInUse(const std::string &userNickname);
		std::string	notOnChannel(const std::string &channel);
		std::string	noSuchChannel(const std::string &channel);
		std::string	notRegistered(const std::string &userNickname);
		std::string	chanOpPrivsNeeded(const std::string &channelName);
		std::string	unknownMode(const std::string &mode);
		std::string	inviteOnlyChannel(const std::string &channel);
		std::string	badChannelKey(const std::string &channel);
		std::string	channelIsFull(const std::string &channel);
		std::string	userNotInChannel(const std::string &target, const std::string &channel);
		std::string	cannotSendToChannel(const std::string &channel);
		std::string	userOnChannel(const std::string &user, const std::string &channel);
		std::string	unknownCommand(const std::string &command);
	
		// std::string kickMessage(const std::string &operatorName, const std::string &target, const std::string &channel, const std::string &reason);
		// std::string inviteMessage(const std::string &operatorName, const std::string &target, const std::string &channel);

		// std::string topicReply(const std::string &channel, const std::string &topic);
		// std::string topicChange(const std::string &operatorName, const std::string &channel, const std::string &newTopic);

		// std::string modeChange(const std::string &channel, const std::string &mode, const std::string &parameter);
	
		// std::string error(const std::string &errorCode, const std::string &message);
		
		
	private:
		std::map<std::string, std::string>	_replyTemplates;

		void		_initializeTemplates();
		std::string	_formatReply(const std::string &templateStr, const std::vector<std::string> &args) const;
};

// helper function for generating argument lists
std::vector<std::string>	makeArgs(const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "");

#endif // REPLY_HPP


