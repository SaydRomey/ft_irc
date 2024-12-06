// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   reply_functions.cpp                                :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/12/05 19:23:58 by cdumais           #+#    #+#             */
// /*   Updated: 2024/12/05 19:42:06 by cdumais          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// # include "ReplyTypes.hpp"

// # include <string>
// # include <vector>

// class Reply
// {
// 	public:
// 		Reply();
// 		~Reply();

// 		std::string	reply(ReplyType key, const std::vector<std::string> &args) const;
// 		std::string	reply(int key, const std::vector<std::string> &args) const;
// 		std::string	reply(ReplyType key, const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "");
// 		std::string	reply(int key, const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "");
	
// 	// General Replies (Authentication)
// 		std::string welcome(const std::string &userNickname) const;
// 		std::string yourHost(const std::string &userNickname, const std::string &serverName, const std::string &version) const;
// 		std::string serverCreated(const std::string &userNickname, const std::string &creationDate) const;
// 		std::string myInfo(const std::string &userNickname, const std::string &serverName, const std::string &version,
//                    const std::string &availableUserModes, const std::string &availableChannelModes) const;

// 	// Channel Management Replies
// 		std::string channelModeIs(const std::string &channel, const std::string &modes) const;
// 		std::string noTopic(const std::string &channel) const;
// 		std::string topic(const std::string &channel, const std::string &topic) const;
// 		std::string inviting(const std::string &inviter, const std::string &invitee) const;

// 	// Messaging Replies
// 		std::string privateMessage(const std::string &sender, const std::string &receiver, const std::string &message) const;
// 		std::string channelMessage(const std::string &sender, const std::string &channel, const std::string &message) const;

// 	// Error Replies (Authentication)
// 		std::string noNicknameGiven() const;
// 		std::string erroneousNickname(const std::string &nickname) const;
// 		std::string nicknameInUse(const std::string &nickname) const;
// 		std::string needMoreParams(const std::string &command) const;
// 		std::string alreadyRegistered() const;

// 	// Error Replies (Channel Management)
// 		std::string noSuchChannel(const std::string &channel) const;
// 		std::string cannotSendToChannel(const std::string &channel) const;
// 		std::string tooManyChannels(const std::string &channel) const;
// 		std::string tooManyTargets(const std::string &target) const;
// 		std::string userNotInChannel(const std::string &user, const std::string &channel) const;
// 		std::string notOnChannel(const std::string &channel) const;
// 		std::string userOnChannel(const std::string &user, const std::string &channel) const;
// 		std::string keySet(const std::string &channel) const;
// 		std::string channelIsFull(const std::string &channel) const;
// 		std::string unknownMode(const std::string &mode) const;
// 		std::string inviteOnlyChannel(const std::string &channel) const;
// 		std::string badChannelKey(const std::string &channel) const;
// 		std::string badChannelMask(const std::string &channel) const;
// 		std::string chanOpPrivsNeeded(const std::string &channel) const;

// 	// Error Replies (Messaging)
// 		std::string noSuchNick(const std::string &nickname) const;
// 		std::string noRecipient(const std::string &command) const;
// 		std::string noTextToSend() const;

// 	// other ?
// 		std::string	unknownCommand(const std::string &command) const;
// };

// // helper function for generating argument lists
// std::vector<std::string>	makeArgs(const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "");




// // /* ************************************************************************** */ // General replies

// // Authentication

// std::string	Reply::welcome(const std::string &userNickname) const
// {
// 	// std::vector<std::string>	args;
// 	// args.push_back(userNickname);
// 	// args.push_back(userNickname);
// 	// return (reply(RPL_WELCOME, args));
	
// 	return (reply(RPL_WELCOME, makeArgs(userNickname, userNickname)));
// }

// std::string Reply::yourHost(const std::string &userNickname, const std::string &serverName, const std::string &version) const
// {
//     return (reply(RPL_YOURHOST, makeArgs(userNickname, serverName, version)));
// }

// std::string Reply::serverCreated(const std::string &userNickname, const std::string &creationDate) const
// {
//     return (reply(RPL_CREATED, makeArgs(userNickname, creationDate)));
// }

// std::string Reply::myInfo(const std::string &userNickname, const std::string &serverName, const std::string &version,
//                            const std::string &availableUserModes, const std::string &availableChannelModes) const
// {
//     return (reply(RPL_MYINFO, makeArgs(userNickname, serverName, version, availableUserModes + " " + availableChannelModes)));
// }


// // Channel management

// std::string Reply::channelModeIs(const std::string &channel, const std::string &modes) const
// {
//     return (reply(RPL_CHANNELMODEIS, makeArgs(channel, modes)));
// }

// std::string Reply::noTopic(const std::string &channel) const
// {
//     return (reply(RPL_NOTOPIC, makeArgs(channel)));
// }

// std::string Reply::topic(const std::string &channel, const std::string &topic) const
// {
//     return (reply(RPL_TOPIC, makeArgs(channel, topic)));
// }

// std::string Reply::inviting(const std::string &inviter, const std::string &invitee) const
// {
//     return (reply(RPL_INVITING, makeArgs(inviter, invitee)));
// }


// // Messaging ** these are not numeric replies, but lets try this for now..

// std::string Reply::privateMessage(const std::string &sender, const std::string &receiver, const std::string &message) const
// {
// 	return (":" + sender + " PRIVMSG " + receiver + " :" + message);
	
// }

// std::string Reply::channelMessage(const std::string &sender, const std::string &channel, const std::string &message) const
// {
// 	return (":" + sender + " PRIVMSG " + channel + " :" + message);
// }


// // Error replies (authentication)

// std::string Reply::noNicknameGiven() const
// {
//     return (reply(ERR_NONICKNAMEGIVEN, makeArgs()));
// }

// std::string Reply::erroneousNickname(const std::string &nickname) const
// {
//     return (reply(ERR_ERRONEUSNICKNAME, makeArgs(nickname)));
// }

// std::string Reply::nicknameInUse(const std::string &nickname) const
// {
//     return (reply(ERR_NICKNAMEINUSE, makeArgs(nickname)));
// }

// std::string Reply::needMoreParams(const std::string &command) const
// {
//     return (reply(ERR_NEEDMOREPARAMS, makeArgs(command)));
// }

// std::string Reply::alreadyRegistered() const
// {
//     return (reply(ERR_ALREADYREGISTERED, makeArgs()));
// }


// // Error replies (channel management)

// std::string Reply::noSuchChannel(const std::string &channel) const
// {
//     return (reply(ERR_NOSUCHCHANNEL, makeArgs(channel)));
// }

// std::string Reply::cannotSendToChannel(const std::string &channel) const
// {
//     return (reply(ERR_CANNOTSENDTOCHAN, makeArgs(channel)));
// }

// std::string Reply::tooManyChannels(const std::string &channel) const
// {
//     return (reply(ERR_TOOMANYCHANNELS, makeArgs(channel)));
// }

// std::string Reply::tooManyTargets(const std::string &target) const
// {
//     return (reply(ERR_TOOMANYTARGETS, makeArgs(target)));
// }

// std::string Reply::userNotInChannel(const std::string &user, const std::string &channel) const
// {
//     return (reply(ERR_USERNOTINCHANNEL, makeArgs(user, channel)));
// }

// std::string Reply::notOnChannel(const std::string &channel) const
// {
//     return (reply(ERR_NOTONCHANNEL, makeArgs(channel)));
// }

// std::string Reply::userOnChannel(const std::string &user, const std::string &channel) const
// {
//     return (reply(ERR_USERONCHANNEL, makeArgs(user, channel)));
// }

// std::string Reply::keySet(const std::string &channel) const
// {
//     return (reply(ERR_KEYSET, makeArgs(channel)));
// }

// std::string Reply::channelIsFull(const std::string &channel) const
// {
//     return (reply(ERR_CHANNELISFULL, makeArgs(channel)));
// }

// std::string Reply::unknownMode(const std::string &mode) const
// {
//     return (reply(ERR_UNKNOWNMODE, makeArgs(mode)));
// }

// std::string Reply::inviteOnlyChannel(const std::string &channel) const
// {
//     return (reply(ERR_INVITEONLYCHAN, makeArgs(channel)));
// }

// std::string Reply::badChannelKey(const std::string &channel) const
// {
//     return (reply(ERR_BADCHANNELKEY, makeArgs(channel)));
// }

// std::string Reply::badChannelMask(const std::string &channel) const
// {
//     return (reply(ERR_BADCHANMASK, makeArgs(channel)));
// }

// std::string Reply::chanOpPrivsNeeded(const std::string &channel) const
// {
//     return (reply(ERR_CHANOPRIVSNEEDED, makeArgs(channel)));
// }


// // Error replies (messaging)

// std::string Reply::noSuchNick(const std::string &nickname) const
// {
//     return (reply(ERR_NOSUCHNICK, makeArgs(nickname)));
// }

// std::string Reply::noRecipient(const std::string &command) const
// {
//     return (reply(ERR_NORECIPIENT, makeArgs(command)));
// }


// std::string Reply::noTextToSend() const
// {
//     return (reply(ERR_NOTEXTTOSEND, makeArgs()));
// }

// // other ?

// std::string	Reply::unknownCommand(const std::string &command) const
// {
// 	return (reply(ERR_UNKNOWNCOMMAND, makeArgs(command)));
// }

// /* ************************************************************************** */ // helper functions

// /*
// Helper function to build argument list
// */
// std::vector<std::string>	makeArgs(const std::string &arg1, const std::string &arg2, const std::string &arg3, const std::string &arg4)
// {
// 	std::vector<std::string>	args;
	
// 	if (!arg1.empty())
// 		args.push_back(arg1);
// 	if (!arg2.empty())
// 		args.push_back(arg2);
// 	if (!arg3.empty())
// 		args.push_back(arg3);
// 	if (!arg4.empty())
// 		args.push_back(arg4);
	
// 	return (args);
// }

