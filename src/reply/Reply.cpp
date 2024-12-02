/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 02:05:32 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/02 00:53:39 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Reply.hpp"
#include <sstream>
#include <stdexcept>

const std::string Reply::SERVER_NAME = "ircserv";

Reply::Reply()
{
	_initializeTemplates();
}

Reply::~Reply() {}

void Reply::_initializeTemplates()
{
	// General server replies
	_replyTemplates[RPL_WELCOME] = ":" + SERVER_NAME + " 001 %s :Welcome to Fun Times City, dear %s!";
	_replyTemplates[RPL_AUTH_FAILED] = ":" + SERVER_NAME + " 464 * :Authentication failed: %s.";
	_replyTemplates[RPL_JOIN_SUCCESS] = ":" + SERVER_NAME + " 332 %s :Joined channel %s.";
	_replyTemplates[RPL_PRIVATE_MSG] = ":%s PRIVMSG %s :%s"; // sender's nickname as prefix
	_replyTemplates[RPL_CHANNEL_MSG] = ":%s PRIVMSG %s :%s"; // sender's nickname as prefix
	_replyTemplates[RPL_KICK] = ":" + SERVER_NAME + " KICK %s %s :%s";
	_replyTemplates[RPL_INVITE] = ":" + SERVER_NAME + " INVITE %s %s";
	_replyTemplates[RPL_TOPIC] = ":" + SERVER_NAME + " 332 %s :%s";
	_replyTemplates[RPL_MODE] = ":" + SERVER_NAME + " MODE %s %s";

	// Error replies
	_replyTemplates[ERR_NEEDMOREPARAMS] = ":" + SERVER_NAME + " 461 %s %s :Not enough parameters.";
	_replyTemplates[ERR_ALREADYREGISTERED] = ":" + SERVER_NAME + " 462 %s :You may not reregister.";
	_replyTemplates[ERR_NICKNAMEINUSE] = ":" + SERVER_NAME + " 433 * %s :Nickname is already in use.";
	_replyTemplates[ERR_NOTONCHANNEL] = ":" + SERVER_NAME + " 442 %s %s :You're not on that channel.";
	_replyTemplates[ERR_NOSUCHCHANNEL] = ":" + SERVER_NAME + " 403 %s :No such channel.";
	_replyTemplates[ERR_NOTREGISTERED] = ":" + SERVER_NAME + " 451 %s :You have not registered.";
	_replyTemplates[ERR_CHANOPRIVSNEEDED] = ":" + SERVER_NAME + " 482 %s %s :You're not a channel operator.";
	_replyTemplates[ERR_UNKNOWNMODE] = ":" + SERVER_NAME + " 472 %s :is unknown mode char to me.";
	_replyTemplates[ERR_INVITEONLYCHAN] = ":" + SERVER_NAME + " 473 %s :Cannot join channel (+i).";
	_replyTemplates[ERR_BADCHANNELKEY] = ":" + SERVER_NAME + " 475 %s :Cannot join channel (+k).";
	_replyTemplates[ERR_CHANNELISFULL] = ":" + SERVER_NAME + " 471 %s :Cannot join channel (+l).";
	_replyTemplates[ERR_USERNOTINCHANNEL] = ":" + SERVER_NAME + " 441 %s %s :They aren't on that channel.";
	_replyTemplates[ERR_CANNOTSENDTOCHAN] = ":" + SERVER_NAME + " 404 %s %s :Cannot send to channel.";
	_replyTemplates[ERR_USERONCHANNEL] = ":" + SERVER_NAME + " 443 %s %s :is already on channel.";
	_replyTemplates[ERR_UNKNOWNCOMMAND] = ":" + SERVER_NAME + " 421 %s %s :Unknown command.";
}

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

std::string	Reply::welcome(const std::string &userNickname) const
{
	// std::vector<std::string>	args;
	// args.push_back(userNickname);
	// args.push_back(userNickname);
	// return (generateReply(RPL_WELCOME, args));
	
	return (generateReply(RPL_WELCOME, makeArgs(userNickname, userNickname)));
	// return (_formatReply(_replyTemplates[RPL_WELCOME], makeArgs(userNickname, userNickname)))
}

std::string	Reply::authenticationFailed(const std::string &reason) const
{
	return (generateReply(RPL_AUTH_FAILED, makeArgs(reason)));
}

std::string	Reply::joinSuccess(const std::string &channelName) const
{
	return (generateReply(RPL_JOIN_SUCCESS, makeArgs(channelName)));
}

std::string	Reply::privateMessage(const std::string &sender, const std::string &receiver, const std::string &message) const
{
	return (generateReply(RPL_PRIVATE_MSG, makeArgs(sender, receiver, message)));
}

std::string	Reply::channelMessage(const std::string &sender, const std::string &channel, const std::string &message) const
{
	return (generateReply(RPL_CHANNEL_MSG, makeArgs(sender, channel, message)));
}

// std::string	Reply::kickMessage(const std::string &operatorName, const std::string &target, const std::string &channel, const std::string &reason) const;

// std::string	Reply::inviteMessage(const std::string &operatorName, const std::string &target, const std::string &channel) const;

// std::string	Reply::topicMessage(const std::string &channel, const std::string &topic) const;

// std::string	Reply::modeChange(const std::string &channel, const std::string &mode, const std::string &parameter) const;


/* ************************************************************************** */ // Error replies

std::string	Reply::needMoreParams(const std::string &command) const
{
	return (generateReply(ERR_NEEDMOREPARAMS, makeArgs(command)));
}
// std::string	Reply::alreadyRegistered(const std::string &userNickname) const;

std::string	Reply::nicknameInUse(const std::string &userNickname) const
{
	return (generateReply(ERR_NICKNAMEINUSE, makeArgs(userNickname)));
}

// std::string	Reply::notOnChannel(const std::string &channel) const;

// std::string	Reply::noSuchChannel(const std::string &channel) const;

// std::string	Reply::notRegistered(const std::string &userNickname) const;

std::string	Reply::chanOpPrivsNeeded(const std::string &channelName) const
{
	return (generateReply(ERR_CHANOPRIVSNEEDED, makeArgs(channelName)));
}

// std::string	Reply::unknownMode(const std::string &mode) const;

// std::string	Reply::inviteOnlyChannel(const std::string &channel) const;

// std::string	Reply::badChannelKey(const std::string &channel) const;

// std::string	Reply::channelIsFull(const std::string &channel) const;

// std::string	Reply::userNotInChannel(const std::string &target, const std::string &channel) const;

// std::string	Reply::cannotSendToChannel(const std::string &channel) const;

// std::string	Reply::userOnChannel(const std::string &user, const std::string &channel) const;

// std::string	Reply::unknownCommand(const std::string &command) const;

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
