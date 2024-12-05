/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 02:05:32 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/04 20:58:22 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Reply.hpp"
#include <sstream>
#include <stdexcept>

const std::string Reply::SERVER_NAME = "ircserv";

Reply::Reply()
{
	// _initializeTemplates(); // backup if .ipp does not work
	
	// map init should be included here from the .ipp file ? (to test, then remove initTemplates func ?)
	#include "ReplyTemplates.ipp" // init template reply strings
}

Reply::~Reply() {}

// void Reply::_initializeTemplates()
// {
// 	// ========================
// 	// AUTHENTICATION COMMANDS
// 	// ========================
// 	_replyTemplates[RPL_WELCOME] = ":" + SERVER_NAME + " 001 %s :Welcome to the Internet Relay Network %s!";
// 	_replyTemplates[RPL_YOURHOST] = ":" + SERVER_NAME + " 002 %s :Your host is %s, running version %s";
// 	_replyTemplates[RPL_CREATED] = ":" + SERVER_NAME + " 003 %s :This server was created %s";
// 	_replyTemplates[RPL_MYINFO] = ":" + SERVER_NAME + " 004 %s %s %s %s %s";

// 	_replyTemplates[ERR_NONICKNAMEGIVEN] = ":" + SERVER_NAME + " 431 :No nickname given";
// 	_replyTemplates[ERR_ERRONEUSNICKNAME] = ":" + SERVER_NAME + " 432 * %s :Erroneous nickname";
// 	_replyTemplates[ERR_NICKNAMEINUSE] = ":" + SERVER_NAME + " 433 * %s :Nickname is already in use";
// 	_replyTemplates[ERR_NEEDMOREPARAMS] = ":" + SERVER_NAME + " 461 %s :Not enough parameters";
// 	_replyTemplates[ERR_ALREADYREGISTERED] = ":" + SERVER_NAME + " 462 :You may not reregister";

// 	// ==========================
// 	// CHANNEL MANAGEMENT COMMANDS
// 	// ==========================
// 	_replyTemplates[RPL_CHANNELMODEIS] = ":" + SERVER_NAME + " 324 %s :%s";
// 	_replyTemplates[RPL_NOTOPIC] = ":" + SERVER_NAME + " 331 %s :No topic is set";
// 	_replyTemplates[RPL_TOPIC] = ":" + SERVER_NAME + " 332 %s %s :%s";
// 	_replyTemplates[RPL_INVITING] = ":" + SERVER_NAME + " 341 %s %s";

// 	_replyTemplates[ERR_NOSUCHCHANNEL] = ":" + SERVER_NAME + " 403 %s :No such channel";
// 	_replyTemplates[ERR_CANNOTSENDTOCHAN] = ":" + SERVER_NAME + " 404 %s :Cannot send to channel";
// 	_replyTemplates[ERR_TOOMANYCHANNELS] = ":" + SERVER_NAME + " 405 %s :You have joined too many channels";
// 	_replyTemplates[ERR_TOOMANYTARGETS] = ":" + SERVER_NAME + " 407 %s :Duplicate recipients. No message delivered";
// 	_replyTemplates[ERR_USERNOTINCHANNEL] = ":" + SERVER_NAME + " 441 %s %s :They aren't on that channel";
// 	_replyTemplates[ERR_NOTONCHANNEL] = ":" + SERVER_NAME + " 442 %s :You're not on that channel";
// 	_replyTemplates[ERR_USERONCHANNEL] = ":" + SERVER_NAME + " 443 %s %s :is already on channel";

// 	_replyTemplates[ERR_KEYSET] = ":" + SERVER_NAME + " 467 %s :Channel key already set";
// 	_replyTemplates[ERR_CHANNELISFULL] = ":" + SERVER_NAME + " 471 %s :Cannot join channel (+l)";
// 	_replyTemplates[ERR_UNKNOWNMODE] = ":" + SERVER_NAME + " 472 %s :is unknown mode char to me";
// 	_replyTemplates[ERR_INVITEONLYCHAN] = ":" + SERVER_NAME + " 473 %s :Cannot join channel (+i)";
// 	_replyTemplates[ERR_BADCHANNELKEY] = ":" + SERVER_NAME + " 475 %s :Cannot join channel (+k)";
// 	_replyTemplates[ERR_BADCHANMASK] = ":" + SERVER_NAME + " 476 %s :Bad channel mask";
// 	_replyTemplates[ERR_CHANOPRIVSNEEDED] = ":" + SERVER_NAME + " 482 %s :You're not channel operator";

// 	// =======================
// 	// MESSAGING COMMANDS
// 	// =======================
// 	_replyTemplates[ERR_NOSUCHNICK] = ":" + SERVER_NAME + " 401 %s :No such nick/channel";
	// _replyTemplates[ERR_NORECIPIENT] = ":" + SERVER_NAME + " 411 :No recipient given (%s)";
	// _replyTemplates[ERR_NOTEXTTOSEND] = ":" + SERVER_NAME + " 412 :No text to send";


// 	// =======================
// 	// OTHER ?
// 	// =======================
	// _replyTemplates[ERR_UNKNOWNCOMMAND] = ":" + SERVER_NAME + " 421 %s :Unknown command.";

// }

/*
format a reply based on the template and arguments
** using ostringstream **

loop through each character in template string
replace occurrences of "%s" with corresponding argument from the vector
append non-placeholder characters directly to the result
exception thrown if mismatched placeholders and arguments..
*/
std::string	Reply::_formatReply(const std::string &templateStr, const std::vector<std::string> &args) const
{
	std::ostringstream	oss;
	size_t				argIndex = 0;
	
	size_t	i = 0;
	while (i < templateStr.size()) // process the template string and replace placeholders
	{
		if (templateStr[i] == '%' && i + 1 < templateStr.size() && templateStr[i + 1] == 's')
		{
			if (argIndex >= args.size())
				throw (std::runtime_error("Not enough arguments for reply template: " + templateStr));
			oss << args[argIndex++];
			++i; // skip 's' after '%'
		}
		else
			oss << templateStr[i];
		++i;
	}
	if (argIndex < args.size())
		throw (std::runtime_error("Too many arguments for reply template: " + templateStr));

	return (oss.str());
}

/*
generic method to generate replies using templates
** use key in '_replyTemplates' map,
** use helper function 'makeArgs()' to generate variadic vector for 'args' parameter
*/
std::string	Reply::generateReply(ReplyType key, const std::vector<std::string> &args) const
{
	std::map<ReplyType, std::string>::const_iterator	it = _replyTemplates.find(key);

	if (it == _replyTemplates.end())
	{
		throw (std::runtime_error("Unknown reply key"));
	}
	
	return (_formatReply(it->second, args));
}

/* ************************************************************************** */ // General replies

// Authentication

std::string	Reply::welcome(const std::string &userNickname) const
{
	// std::vector<std::string>	args;
	// args.push_back(userNickname);
	// args.push_back(userNickname);
	// return (generateReply(RPL_WELCOME, args));
	
	return (generateReply(RPL_WELCOME, makeArgs(userNickname, userNickname)));
}

std::string Reply::yourHost(const std::string &userNickname, const std::string &serverName, const std::string &version) const
{
    return (generateReply(RPL_YOURHOST, makeArgs(userNickname, serverName, version)));
}

std::string Reply::serverCreated(const std::string &userNickname, const std::string &creationDate) const
{
    return (generateReply(RPL_CREATED, makeArgs(userNickname, creationDate)));
}

std::string Reply::myInfo(const std::string &userNickname, const std::string &serverName, const std::string &version,
                           const std::string &availableUserModes, const std::string &availableChannelModes) const
{
    return (generateReply(RPL_MYINFO, makeArgs(userNickname, serverName, version, availableUserModes + " " + availableChannelModes)));
}


// Channel management

std::string Reply::channelModeIs(const std::string &channel, const std::string &modes) const
{
    return (generateReply(RPL_CHANNELMODEIS, makeArgs(channel, modes)));
}

std::string Reply::noTopic(const std::string &channel) const
{
    return (generateReply(RPL_NOTOPIC, makeArgs(channel)));
}

std::string Reply::topic(const std::string &channel, const std::string &topic) const
{
    return (generateReply(RPL_TOPIC, makeArgs(channel, topic)));
}

std::string Reply::inviting(const std::string &inviter, const std::string &invitee) const
{
    return (generateReply(RPL_INVITING, makeArgs(inviter, invitee)));
}


// Messaging ** these are not numeric replies, but lets try this for now..

std::string Reply::privateMessage(const std::string &sender, const std::string &receiver, const std::string &message) const
{
	return (":" + sender + " PRIVMSG " + receiver + " :" + message);
	
}

std::string Reply::channelMessage(const std::string &sender, const std::string &channel, const std::string &message) const
{
	return (":" + sender + " PRIVMSG " + channel + " :" + message);
}


// Error replies (authentication)

std::string Reply::noNicknameGiven() const
{
    return (generateReply(ERR_NONICKNAMEGIVEN, makeArgs()));
}

std::string Reply::erroneousNickname(const std::string &nickname) const
{
    return (generateReply(ERR_ERRONEUSNICKNAME, makeArgs(nickname)));
}

std::string Reply::nicknameInUse(const std::string &nickname) const
{
    return (generateReply(ERR_NICKNAMEINUSE, makeArgs(nickname)));
}

std::string Reply::needMoreParams(const std::string &command) const
{
    return (generateReply(ERR_NEEDMOREPARAMS, makeArgs(command)));
}

std::string Reply::alreadyRegistered() const
{
    return (generateReply(ERR_ALREADYREGISTERED, makeArgs()));
}


// Error replies (channel management)

std::string Reply::noSuchChannel(const std::string &channel) const
{
    return (generateReply(ERR_NOSUCHCHANNEL, makeArgs(channel)));
}

std::string Reply::cannotSendToChannel(const std::string &channel) const
{
    return (generateReply(ERR_CANNOTSENDTOCHAN, makeArgs(channel)));
}

std::string Reply::tooManyChannels(const std::string &channel) const
{
    return (generateReply(ERR_TOOMANYCHANNELS, makeArgs(channel)));
}

std::string Reply::tooManyTargets(const std::string &target) const
{
    return (generateReply(ERR_TOOMANYTARGETS, makeArgs(target)));
}

std::string Reply::userNotInChannel(const std::string &user, const std::string &channel) const
{
    return (generateReply(ERR_USERNOTINCHANNEL, makeArgs(user, channel)));
}

std::string Reply::notOnChannel(const std::string &channel) const
{
    return (generateReply(ERR_NOTONCHANNEL, makeArgs(channel)));
}

std::string Reply::userOnChannel(const std::string &user, const std::string &channel) const
{
    return (generateReply(ERR_USERONCHANNEL, makeArgs(user, channel)));
}

std::string Reply::keySet(const std::string &channel) const
{
    return (generateReply(ERR_KEYSET, makeArgs(channel)));
}

std::string Reply::channelIsFull(const std::string &channel) const
{
    return (generateReply(ERR_CHANNELISFULL, makeArgs(channel)));
}

std::string Reply::unknownMode(const std::string &mode) const
{
    return (generateReply(ERR_UNKNOWNMODE, makeArgs(mode)));
}

std::string Reply::inviteOnlyChannel(const std::string &channel) const
{
    return (generateReply(ERR_INVITEONLYCHAN, makeArgs(channel)));
}

std::string Reply::badChannelKey(const std::string &channel) const
{
    return (generateReply(ERR_BADCHANNELKEY, makeArgs(channel)));
}

std::string Reply::badChannelMask(const std::string &channel) const
{
    return (generateReply(ERR_BADCHANMASK, makeArgs(channel)));
}

std::string Reply::chanOpPrivsNeeded(const std::string &channel) const
{
    return (generateReply(ERR_CHANOPRIVSNEEDED, makeArgs(channel)));
}


// Error replies (messaging)

std::string Reply::noSuchNick(const std::string &nickname) const
{
    return (generateReply(ERR_NOSUCHNICK, makeArgs(nickname)));
}

std::string Reply::noRecipient(const std::string &command) const
{
    return (generateReply(ERR_NORECIPIENT, makeArgs(command)));
}


std::string Reply::noTextToSend() const
{
    return (generateReply(ERR_NOTEXTTOSEND, makeArgs()));
}

// other ?

std::string	Reply::unknownCommand(const std::string &command) const
{
	return (generateReply(ERR_UNKNOWNCOMMAND, makeArgs(command)));
}

/* ************************************************************************** */ // helper functions

/*
Helper function to build argument list
*/
std::vector<std::string>	makeArgs(const std::string &arg1, const std::string &arg2, const std::string &arg3, const std::string &arg4)
{
	std::vector<std::string>	args;
	
	if (!arg1.empty())
		args.push_back(arg1);
	if (!arg2.empty())
		args.push_back(arg2);
	if (!arg3.empty())
		args.push_back(arg3);
	if (!arg4.empty())
		args.push_back(arg4);
	
	return (args);
}

/*	example usage

#include "Reply.hpp"
#include <iostream>

void	test_reply(void)
{
	Reply	reply;

	try
	{
		std::cout << reply.welcome("HomeBoy"); << std::endl;
		std::cout << reply.nicknameInUse("HomeBoy") << std::endl;
		std::cout << reply.privateMessage("senderPerson", "recieverPerson", "Hello my friend!") << std::endl;
		std::cout << reply.needMoreParams("JOIN") << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

*/
