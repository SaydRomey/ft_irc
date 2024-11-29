/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 02:05:32 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/29 12:42:00 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Reply.hpp"
#include <sstream>
#include <stdexcept>
// #include <cstdio> // if we use snprintf

Reply::Reply()
{
	_initializeTemplates();
}

Reply::~Reply() {}

void Reply::_initializeTemplates()
{
	// General server replies
	_replyTemplates["RPL_WELCOME"] = ":" SERVER_NAME " 001 %s :Welcome to the IRC network, %s!";
	_replyTemplates["RPL_AUTH_FAILED"] = ":" SERVER_NAME " 464 * :Authentication failed: %s.";
	_replyTemplates["RPL_JOIN_SUCCESS"] = ":" SERVER_NAME " 332 %s :Joined channel %s.";
	_replyTemplates["RPL_PRIVATE_MSG"] = ":%s PRIVMSG %s :%s"; // sender's nickname as prefix
	_replyTemplates["RPL_CHANNEL_MSG"] = ":%s PRIVMSG %s :%s"; // sender's nickname as prefix
	_replyTemplates["RPL_KICK"] = ":" SERVER_NAME " KICK %s %s :%s";
	_replyTemplates["RPL_INVITE"] = ":" SERVER_NAME " INVITE %s %s";
	_replyTemplates["RPL_TOPIC"] = ":" SERVER_NAME " 332 %s :%s";
	_replyTemplates["RPL_MODE"] = ":" SERVER_NAME " MODE %s %s";

	// Error replies
	_replyTemplates["ERR_NEEDMOREPARAMS"] = ":" SERVER_NAME " 461 %s %s :Not enough parameters.";
	_replyTemplates["ERR_ALREADYREGISTERED"] = ":" SERVER_NAME " 462 %s :You may not reregister.";
	_replyTemplates["ERR_NICKNAMEINUSE"] = ":" SERVER_NAME " 433 * %s :Nickname is already in use.";
	_replyTemplates["ERR_NOTONCHANNEL"] = ":" SERVER_NAME " 442 %s %s :You're not on that channel.";
	_replyTemplates["ERR_NOSUCHCHANNEL"] = ":" SERVER_NAME " 403 %s :No such channel.";
	_replyTemplates["ERR_NOTREGISTERED"] = ":" SERVER_NAME " 451 %s :You have not registered.";
	_replyTemplates["ERR_CHANOPRIVSNEEDED"] = ":" SERVER_NAME " 482 %s %s :You're not a channel operator.";
	_replyTemplates["ERR_UNKNOWNMODE"] = ":" SERVER_NAME " 472 %s :is unknown mode char to me.";
	_replyTemplates["ERR_INVITEONLYCHAN"] = ":" SERVER_NAME " 473 %s :Cannot join channel (+i).";
	_replyTemplates["ERR_BADCHANNELKEY"] = ":" SERVER_NAME " 475 %s :Cannot join channel (+k).";
	_replyTemplates["ERR_CHANNELISFULL"] = ":" SERVER_NAME " 471 %s :Cannot join channel (+l).";
	_replyTemplates["ERR_USERNOTINCHANNEL"] = ":" SERVER_NAME " 441 %s %s :They aren't on that channel.";
	_replyTemplates["ERR_CANNOTSENDTOCHAN"] = ":" SERVER_NAME " 404 %s %s :Cannot send to channel.";
	_replyTemplates["ERR_USERONCHANNEL"] = ":" SERVER_NAME " 443 %s %s :is already on channel.";
	_replyTemplates["ERR_UNKNOWNCOMMAND"] = ":" SERVER_NAME " 421 %s %s :Unknown command.";
}

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
	size_t	argIndex = 0;
	
	size_t	i = 0;
	while (i < templateStr.size())
	{
		if (templateStr[i] == '%' && i + 1 < templateStr.size() && templateStr[i + 1] == 's')
		{
			if (argIndex >= args.size())
			{
				throw (std::runtime_error("Not enough arguments for reply template: " + templateStr));
			}
			oss << args[argIndex++];
			++i; // skip 's' after '%'
		}
		else
		{
			oss << templateStr[i];
		}
		++i;
	}
	if (argIndex < args.size())
	{
		throw (std::runtime_error("Too many arguments for reply template: " + templateStr));
	}

	return (oss.str());
}

/*
generic method to generate replies using templates
*/
std::string	Reply::generateReply(const std::string &key, const std::vector<std::string> &args)
{
	std::map<std::string, std::string>::const_iterator	it = _replyTemplates.find(key);

	if (it == _replyTemplates.end())
	{
		throw (std::runtime_error("Reply template not found for key: " + key));
	}
	
	return (_formatReply(it->second, args));
}

/*
if we prefer a variadic version with overloading instead of using vectors..
*/
// std::string	Reply::generateReply(const std::string &key, const std::string &arg1, const std::string &arg2, const std::string &arg3)
// {
// 	std::map<std::string, std::string>::const_iterator	it = _replyTemplates.find(key);
// 	if (it == _replyTemplates.end())
// 	{
// 		throw (std::runtime_error("Reply template not found for key: " + key));
// 	}

// 	const std::string&	templateStr = it->second;
// 	std::ostringstream	oss;
// 	size_t	argIndex = 0;

// 	size_t	i = 0;
// 	while (i < templateStr.size())
// 	{
// 		if (templateStr[i] == '%' && i + 1 < templateStr.size() && templateStr[i + 1] == 's')
// 		{
// 			++i; // skip 's'
// 			switch (argIndex++)
// 			{
// 				case 0:
// 					oss << arg1;
// 					break ;
// 				case 1:
// 					oss << arg2;
// 					break ;
// 				case 2:
// 					oss << arg3;
// 					break ;
// 				default:
// 					throw (std::runtime_error("Too many placeholders in template."));
// 			}
// 		}
// 		else
// 		{
// 			oss << templateStr[i];
// 		}
// 		++i;
// 	}

// 	if (argIndex < 3 && !arg3.empty())
// 	{
// 		throw (std::runtime_error("Too many arguments for template: " + key));
// 	}

// 	return (oss.str());
// }


/*
format a reply based on the template and arguments
** using snprintf **
*/
// std::string	Reply::_formatReply(const std::string &templateStr, const std::vector<std::string> &args) const
// {
// 	char	buffer[512]; //might adjust size if necessary later..
// 	int		result;

// 	// build format string dynamically based on template and arguments
// 	switch (args.size())
// 	{
// 		case 1:
// 			result = snprintf(buffer, sizeof(buffer), templateStr.c_str(), args[0].c_str());
// 			break ;
// 		case 2:
// 			result = snprintf(buffer, sizeof(buffer), templateStr.c_str(), args[0].c_str(), args[1].c_str());
// 			break ;
// 		case 3:
// 			result = snprintf(buffer, sizeof(buffer), templateStr.c_str(), args[0].c_str(), args[1].c_str(), args[2].c_str());
// 			break ;
// 		case 4:
// 			result = snprintf(buffer, sizeof(buffer), templateStr.c_str(), args[0].c_str(), args[1].c_str(), args[2].c_str(), args[3].c_str());
// 			break ;
// 		default:
// 			throw (std::runtime_error("Unsupported number of arguments for reply formatting..."));
// 	}
	
// 	if (result < 0 || result >= static_cast<int>(sizeof(buffer)))
// 	{
// 		throw (std::runtime_error("Reply formatting failed or truncated..."));
// 	}

// 	return (std::string(buffer));
// }





std::string	Reply::welcome(const std::string &userNickname)
{
	// std::vector<std::string>	args;
	// args.push_back(serverName);
	// args.push_back(userNickname);
	// args.push_back(userNickname);
	// return (generateReply("WELCOME", args));
	
	return (generateReply("RPL_WELCOME", makeArgs(userNickname, userNickname)));
	// return (_formatReply(_replyTemplates["RPL_WELCOME"], makeArgs(userNickname, userNickname)))
	
	// return (generateReply("WELCOME", std::vector<std::string>{serverName, userNickname, userNickname}));
}

std::string	Reply::authenticationFailed(const std::string &reason)
{
	return (generateReply("RPL_AUTHFAILED", makeArgs(reason)));
}

std::string	Reply::joinSuccess(const std::string &channelName)
{
	return (generateReply("RPL_JOINSUCCESS", makeArgs(channelName)));
}

std::string	Reply::privateMessage(const std::string &sender, const std::string &receiver, const std::string &message)
{
	return (generateReply("RPL_PRIVMSG", makeArgs(sender, receiver, message)));
}

std::string	Reply::channelMessage(const std::string &sender, const std::string &channel, const std::string &message)
{
	return (generateReply("RPL_CHANNELMSG", makeArgs(sender, channel, message)));
}

// std::string	Reply::kickMessage(const std::string &operatorName, const std::string &target, const std::string &channel, const std::string &reason);

// std::string	Reply::inviteMessage(const std::string &operatorName, const std::string &target, const std::string &channel);

// std::string	Reply::topicMessage(const std::string &channel, const std::string &topic);

// std::string	Reply::modeChange(const std::string &channel, const std::string &mode, const std::string &parameter);



std::string	Reply::needMoreParams(const std::string &command)
{
	return (generateReply("ERR_NEEDMOREPARAMS", makeArgs(command)));
}
// std::string	Reply::alreadyRegistered(const std::string &userNickname);
std::string	Reply::nicknameInUse(const std::string &userNickname);
// std::string	Reply::notOnChannel(const std::string &channel);
// std::string	Reply::noSuchChannel(const std::string &channel);
// std::string	Reply::notRegistered(const std::string &userNickname);
std::string	Reply::chanOpPrivsNeeded(const std::string &channelName);
// std::string	Reply::unknownMode(const std::string &mode);
// std::string	Reply::inviteOnlyChannel(const std::string &channel);
// std::string	Reply::badChannelKey(const std::string &channel);
// std::string	Reply::channelIsFull(const std::string &channel);
// std::string	Reply::userNotInChannel(const std::string &target, const std::string &channel);
// std::string	Reply::cannotSendToChannel(const std::string &channel);
// std::string	Reply::userOnChannel(const std::string &user, const std::string &channel);
// std::string	Reply::unknownCommand(const std::string &command);

/*	example usage

#include "Reply.hpp"

void	test_reply(void)
{
	Reply	reply;

	try
	{
		std::string	welcome = reply.welcome("ft_irc", "HomeBoy");
		std::string	nicknameError = reply.nicknameInUse("HomeBoy");
		std::string	privateMessage = reply.privateMessage("senderPerson", "recieverPerson", "Hello my friend!");

		std::cout << welcome << std::endl;
		std::cout << nicknameError << std::endl;
		std::cout << privateMessage << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

*/


/* ************************************************************************** */
/* ************************************************************************** */
/* ************************************************************************** */

/* first idea using snprintf.. this is simple, lets try something more modular..


// Generate replies
std::string	Reply::welcome(const std::string &serverName, const std::string &userNickname)
{
	char	buffer[256];
	snprintf(buffer, sizeof(buffer), _replyTemplates["WELCOME"].c_str(), serverName.c_str(), userNickname.c_str(), userNickname.c_str());
	return (std::string(buffer));
}

std::string	Reply::nicknameInUse(const std::string &userNickname)
{
	char	buffer[256];
	snprintf(buffer, sizeof(buffer), _replyTemplates["NICK_IN_USE"].c_str(), userNickname.c_str());
	return (std::string(buffer));
}

std::string	Reply::authenticationFailed(const std::string &reason)
{
	char	buffer[256];
	snprintf(buffer, sizeof(buffer), _replyTemplates["AUTH_FAILED"].c_str(), reason.c_str());
	return (std::string(buffer));
}

std::string	Reply::joinSuccess(const std::string &channelName)
{
	char	buffer[256];
	snprintf(buffer, sizeof(buffer), _replyTemplates["JOIN_SUCCESS"].c_str(), channelName.c_str());
	return (std::string(buffer));
}

std::string	Reply::privateMessage(const std::string &sender, const std::string &receiver, const std::string &message)
{
	char	buffer[512];
	snprintf(buffer, sizeof(buffer), _replyTemplates["PRIVATE_MSG"].c_str(), sender.c_str(), receiver.c_str(), message.c_str());
	return (std::string(buffer));
}

// ...
*/

/* example usage:

Reply	reply;

std::string	welcome = reply.welcome("ft_irc", "HomeBoy");
std::string	error = reply.nicknameInUse("HomeBoy");
std::string	privateMsg = reply.privateMessage("senderPerson", "recieverPerson", "Hello my friend!");

*/