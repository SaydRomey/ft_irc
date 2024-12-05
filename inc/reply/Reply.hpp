/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 01:35:46 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/04 21:02:48 by cdumais          ###   ########.fr       */
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
		// ==========================
		// AUTHENTICATION COMMANDS
		// ==========================
			RPL_WELCOME,
			RPL_YOURHOST,
			RPL_CREATED,
			RPL_MYINFO,
			ERR_NONICKNAMEGIVEN,
			ERR_ERRONEUSNICKNAME,
			ERR_NICKNAMEINUSE,
			ERR_NEEDMOREPARAMS,
			ERR_ALREADYREGISTERED,

		// ==========================
		// CHANNEL MANAGEMENT COMMANDS
		// ==========================
			RPL_CHANNELMODEIS,
			RPL_NOTOPIC,
			RPL_TOPIC,
			RPL_INVITING,
			ERR_NOSUCHCHANNEL,
			ERR_CANNOTSENDTOCHAN,
			ERR_TOOMANYCHANNELS,
			ERR_TOOMANYTARGETS,
			ERR_USERNOTINCHANNEL,
			ERR_NOTONCHANNEL,
			ERR_USERONCHANNEL,
			ERR_KEYSET,
			ERR_CHANNELISFULL,
			ERR_UNKNOWNMODE,
			ERR_INVITEONLYCHAN,
			ERR_BADCHANNELKEY,
			ERR_BADCHANMASK,
			ERR_CHANOPRIVSNEEDED,

		// =======================
		// MESSAGING COMMANDS
		// =======================
			ERR_NOSUCHNICK,
			ERR_NORECIPIENT,
			ERR_NOTEXTTOSEND,
		
		// =======================
		// OTHER ?
		// =======================
			ERR_UNKNOWNCOMMAND
		};
		
		Reply();
		~Reply();

		std::string	generateReply(ReplyType key, const std::vector<std::string> &args) const; // might change to reply ? (-> Reply rpl; rpl.reply ?..)
		
		// General Replies (Authentication)
		std::string welcome(const std::string &userNickname) const;
		std::string yourHost(const std::string &userNickname, const std::string &serverName, const std::string &version) const;
		std::string serverCreated(const std::string &userNickname, const std::string &creationDate) const;
		std::string myInfo(const std::string &userNickname, const std::string &serverName, const std::string &version,
                   const std::string &availableUserModes, const std::string &availableChannelModes) const;

		// Channel Management Replies
		std::string channelModeIs(const std::string &channel, const std::string &modes) const;
		std::string noTopic(const std::string &channel) const;
		std::string topic(const std::string &channel, const std::string &topic) const;
		std::string inviting(const std::string &inviter, const std::string &invitee) const;

		// Messaging Replies
		std::string privateMessage(const std::string &sender, const std::string &receiver, const std::string &message) const;
		std::string channelMessage(const std::string &sender, const std::string &channel, const std::string &message) const;

		// Error Replies (Authentication)
		std::string noNicknameGiven() const;
		std::string erroneousNickname(const std::string &nickname) const;
		std::string nicknameInUse(const std::string &nickname) const;
		std::string needMoreParams(const std::string &command) const;
		std::string alreadyRegistered() const;

		// Error Replies (Channel Management)
		std::string noSuchChannel(const std::string &channel) const;
		std::string cannotSendToChannel(const std::string &channel) const;
		std::string tooManyChannels(const std::string &channel) const;
		std::string tooManyTargets(const std::string &target) const;
		std::string userNotInChannel(const std::string &user, const std::string &channel) const;
		std::string notOnChannel(const std::string &channel) const;
		std::string userOnChannel(const std::string &user, const std::string &channel) const;
		std::string keySet(const std::string &channel) const;
		std::string channelIsFull(const std::string &channel) const;
		std::string unknownMode(const std::string &mode) const;
		std::string inviteOnlyChannel(const std::string &channel) const;
		std::string badChannelKey(const std::string &channel) const;
		std::string badChannelMask(const std::string &channel) const;
		std::string chanOpPrivsNeeded(const std::string &channel) const;

		// Error Replies (Messaging)
		std::string noSuchNick(const std::string &nickname) const;
		std::string noRecipient(const std::string &command) const;
		std::string noTextToSend() const;

		// other ?
		std::string	unknownCommand(const std::string &command) const;
		
	private:
		static const std::string	SERVER_NAME; // check if this needs to be public to work with .ipp file ..
		std::map<ReplyType, std::string>	_replyTemplates;

		// void		_initializeTemplates();
		std::string	_formatReply(const std::string &templateStr, const std::vector<std::string> &args) const;
};

// helper function for generating argument lists
std::vector<std::string>	makeArgs(const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "");

#endif // REPLY_HPP

/*	**!TODO: lookup which of these expectes replies are not usefull for project scope

- Authentication commands

On successfull authentication:

	001 RPL_WELCOME
	:ircserv 001 [nickname] :Welcome to the Internet Relay Network [nickname]!

	002 RPL_YOURHOST
	:ircserv 002 [nickname] :Your host is [servername], running version [version]

	003 RPL_CREATED
	:ircserv 003 [nickname] :This server was created [date]

	004 RPL_MYINFO
	:ircserv 004 [nickname] [servername] [version] [available user modes] [available channel modes]

PASS

	461 ERR_NEEDMOREPARAMS
	:ircserv 461 [command] :Not enough parameters
	
	462 ERR_ALREADYREGISTERED
	:ircserv 462 :You may not reregister

NICK

	431 ERR_NONICKNAMEGIVEN
	:ircserv 431 :No nickname given

	432 ERR_ERRONEUSNICKNAME
	:ircserv 432 * [nickname] :Erroneous nickname

	433 ERR_NICKNAMEINUSE
	:ircserv 433 * [nickname] :Nickname is already in use

USER

	461 ERR_NEEDMOREPARAMS
	:ircserv 461 USER :Not enough parameters

	462 ERR_ALREADYREGISTERED
	:ircserv 462 :You may not reregister

---

- Channel management commands

JOIN

	403 ERR_NOSUCHCHANNEL
	:ircserv 403 [channel] :No such channel

	405 ERR_TOOMANYCHANNELS
	:ircserv 405 [channel] :You have joined too many channels

	407 ERR_TOOMANYTARGETS
	:ircserv 407 [target] :Duplicate recipients. No message delivered

	461 ERR_NEEDMOREPARAMS
	:ircserv 461 JOIN :Not enough parameters

	471 ERR_CHANNELISFULL
	:ircserv 471 [channel] :Cannot join channel (+l)

	473 ERR_INVITEONLYCHAN
	:ircserv 473 [channel] :Cannot join channel (+i)

	475 ERR_BADCHANNELKEY
	:ircserv 475 [channel] :Cannot join channel (+k)

	476 ERR_BADCHANMASK
	:ircserv 476 [channel] :Bad channel mask

	332 RPL_TOPIC
	:ircserv 332 [nickname] [channel] :[topic]

PART

	403 ERR_NOSUCHCHANNEL
	:ircserv 403 [channel] :No such channel

	442 ERR_NOTONCHANNEL
	:ircserv 442 [channel] :You're not on that channel

	461 ERR_NEEDMOREPARAMS
	:ircserv 461 PART :Not enough parameters

TOPIC

	331 RPL_NOTOPIC
	:ircserv 331 [channel] :No topic is set

	332 RPL_TOPIC
	:ircserv 332 [channel] :[topic]

	442 ERR_NOTONCHANNEL
	:ircserv 442 [channel] :You're not on that channel

	461 ERR_NEEDMOREPARAMS
	:ircserv 461 TOPIC :Not enough parameters

	477 ERR_NOCHANMODES
	:ircserv 477 [channel] :Channel doesn't support modes

	482 ERR_CHANOPRIVSNEEDED
	:ircserv 482 [channel] :You're not channel operator

MODE

	324 RPL_CHANNELMODEIS
	:ircserv 324 [channel] :[modes]

	441 ERR_USERNOTINCHANNEL
	:ircserv 441 [nickname] [channel] :They aren't on that channel

	461 ERR_NEEDMOREPARAMS
	:ircserv 461 MODE :Not enough parameters

	467 ERR_KEYSET
	:ircserv 467 [channel] :Channel key already set

	472 ERR_UNKNOWNMODE
	:ircserv 472 [char] :is unknown mode char to me

	482 ERR_CHANOPRIVSNEEDED
	:ircserv 482 [channel] :You're not channel operator

*(restriction when using 'o' to three mode per MODE command ??)

KICK

	403 ERR_NOSUCHCHANNEL
	:ircserv 403 [channel] :No such channel

	441 ERR_USERNOTINCHANNEL
	:ircserv 441 [nickname] [channel] :They aren't on that channel

	442 ERR_NOTONCHANNEL
	:ircserv 442 [channel] :You're not on that channel

	461 ERR_NEEDMOREPARAMS
	:ircserv 461 KICK :Not enough parameters

	476 ERR_BADCHANMASK
	:ircserv 476 [channel] :Bad channel mask

	482 ERR_CHANOPRIVSNEEDED
	:ircserv 482 [channel] :You're not channel operator

INVITE

	341 RPL_INVITING
	:ircserv 341 [nickname] [channel]

	401 ERR_NOSUCHNICK
	:ircserv 401 [nickname] :No such nick/channel

	442 ERR_NOTONCHANNEL
	:ircserv 442 [channel] :You're not on that channel

	443 ERR_USERONCHANNEL
	:ircserv 443 [nickname] [channel] :is already on channel

	461 ERR_NEEDMOREPARAMS
	:ircserv 461 INVITE :Not enough parameters

	482 ERR_CHANOPRIVSNEEDED
	:ircserv 482 [channel] :You're not channel operator
---

-Messaging commands

PRIVMSG

	401 ERR_NOSUCHNICK
	:ircserv 401 [nickname] :No such nick/channel

	404 ERR_CANNOTSENDTOCHAN
	:ircserv 404 [channel] :Cannot send to channel

	407 ERR_TOOMANYTARGETS
	:ircserv 407 [target] :Duplicate recipients. No message delivered

	411 ERR_NORECIPIENT
	:ircserv 411 :No recipient given (PRIVMSG)

	412 ERR_NOTEXTTOSEND
	:ircserv 412 :No text to send

NOTICE

	N/A (No numeric replies; NOTICE errors are not returned by servers)

---
tocheck:

464 ERR_PASSWDMISMATCH	":Password incorrect"
	->	Returned to indicate a failed attempt at registering a connection
		for which a password was required and was either not given or incorrect.

*/
