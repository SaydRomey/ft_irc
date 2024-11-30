/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 01:35:46 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/30 03:06:22 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_HPP
# define REPLY_HPP

# include <string>
# include <map>
# include <vector>

class Reply
{
	public:
	
		enum ReplyType
		{
			RPL_WELCOME,
			RPL_AUTH_FAILED,
			RPL_JOIN_SUCCESS,
			RPL_PRIVATE_MSG,
			RPL_CHANNEL_MSG,
			RPL_KICK,
			RPL_INVITE,
			RPL_TOPIC,
			RPL_MODE,
			ERR_NEEDMOREPARAMS,
			ERR_ALREADYREGISTERED,
			ERR_NICKNAMEINUSE,
			ERR_NOTONCHANNEL,
			ERR_NOSUCHCHANNEL,
			ERR_NOTREGISTERED,
			ERR_CHANOPRIVSNEEDED,
			ERR_UNKNOWNMODE,
			ERR_INVITEONLYCHAN,
			ERR_BADCHANNELKEY,
			ERR_CHANNELISFULL,
			ERR_USERNOTINCHANNEL,
			ERR_CANNOTSENDTOCHAN,
			ERR_USERONCHANNEL,
			ERR_UNKNOWNCOMMAND
		};
		
		Reply() const;
		~Reply() const;

		std::string	generateReply(ReplyType key, const std::vector<std::string> &args) const;
		
		// General replies
		std::string	welcome(const std::string &userNickname) const;
		std::string	authenticationFailed(const std::string &reason) const;
		
		std::string	joinSuccess(const std::string &channelName) const;
		std::string	privateMessage(const std::string &sender, const std::string &receiver, const std::string &message) const;
		std::string	channelMessage(const std::string &sender, const std::string &channel, const std::string &message) const;
		// std::string	kickMessage(const std::string &operatorName, const std::string &target, const std::string &channel, const std::string &reason) const;
		// std::string	inviteMessage(const std::string &operatorName, const std::string &target, const std::string &channel) const;
		// std::string	topicMessage(const std::string &channel, const std::string &topic) const;
		// std::string	modeChange(const std::string &channel, const std::string &mode, const std::string &parameter) const;
	
		// Error replies
		std::string	needMoreParams(const std::string &command) const;
		// std::string	alreadyRegistered(const std::string &userNickname) const;
		std::string	nicknameInUse(const std::string &userNickname) const;
		// std::string	notOnChannel(const std::string &channel) const;
		// std::string	noSuchChannel(const std::string &channel) const;
		// std::string	notRegistered(const std::string &userNickname) const;
		std::string	chanOpPrivsNeeded(const std::string &channelName) const;
		std::string	unknownMode(const std::string &mode) const;
		std::string	inviteOnlyChannel(const std::string &channel) const;
		std::string	badChannelKey(const std::string &channel) const;
		std::string	channelIsFull(const std::string &channel) const;
		std::string	userNotInChannel(const std::string &target, const std::string &channel) const;
		std::string	cannotSendToChannel(const std::string &channel) const;
		std::string	userOnChannel(const std::string &user, const std::string &channel) const;
		std::string	unknownCommand(const std::string &command) const;
	
		// std::string kickMessage(const std::string &operatorName, const std::string &target, const std::string &channel, const std::string &reason) const;
		// std::string inviteMessage(const std::string &operatorName, const std::string &target, const std::string &channel) const;
		// std::string topicReply(const std::string &channel, const std::string &topic) const;
		// std::string topicChange(const std::string &operatorName, const std::string &channel, const std::string &newTopic) const;
		// std::string modeChange(const std::string &channel, const std::string &mode, const std::string &parameter) const;
		// std::string error(const std::string &errorCode, const std::string &message) const;
		
	private:
		static const std::string	SERVER_NAME;
		std::map<std::string, std::string>	_replyTemplates;

		void		_initializeTemplates();
		std::string	_formatReply(const std::string &templateStr, const std::vector<std::string> &args) const;
};

// helper function for generating argument lists
std::vector<std::string>	makeArgs(const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "");

#endif // REPLY_HPP

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
