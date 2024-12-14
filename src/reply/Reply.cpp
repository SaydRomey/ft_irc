/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 02:05:32 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/13 21:14:17 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Reply.hpp"
#include "parsing_utils.hpp"
#include <ctime>
#include <sstream>
#include <stdexcept>
#include <string>

const std::string Reply::SERVER_NAME = "ircserv";

Reply::Reply()
{
	#include "ReplyTemplates.ipp" // init template reply strings
}

Reply::~Reply() {}

/*
generic method to generate replies using templates
** use key in '_replyTemplates' map,
** use helper function 'makeArgs()' to generate variadic vector for 'args' parameter
*/
std::string	Reply::reply(ReplyType key, const std::vector<std::string> &args) const
{
	std::map<ReplyType, std::string>::const_iterator	it = _replyTemplates.find(key);

	if (it == _replyTemplates.end())
	{
		std::ostringstream	oss;
		oss << "UNKNOWN_REPLY_TYPE(" << static_cast<int>(key) << ")";
		// throw (std::runtime_error(oss.str()));
		// std::cout << oss.str() << std::endl;
		return (oss.str());
	}
	
	return (_formatReply(it->second, args));
}

/*
overload to use an int (like 001 instead of RPL_WELCOME)
*/
std::string	Reply::reply(int key, const std::vector<std::string> &args) const
{
	return (reply(static_cast<ReplyType>(key), args));
}

/*
overloaded reply for direct string arguments
(check if vector one is still needed...)
*/
std::string	Reply::reply(ReplyType key, const std::string &arg1, const std::string &arg2, const std::string &arg3, const std::string &arg4)
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
	
	return (reply(key, args));
}

/*
overload to use an int (like 001 instead of RPL_WELCOME)
*/
std::string	Reply::reply(int key, const std::string &arg1, const std::string &arg2, const std::string &arg3, const std::string &arg4)
{
	return (reply(static_cast<ReplyType>(key), arg1, arg2, arg3, arg4));
}

/*
Formats a reply based on a template and arguments

Uses 'std::ostringstream' to construct the formatted reply
Replace occurrences of "%s" in the template with corresponding argument from the 'args' vector

Parameters:
	`templateStr`: The template string containing placeholders ("%s")
	`args`: A vector of strings to replace placeholders in the template

Returns:
	The formatted reply string if successful
	A dynamically generated error reply using the ERR_UNKNOWNERROR template
	if the number of placeholders and arguments do not match.
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
			if (argIndex >= args.size()) // too few arguments
			{
				throw (std::runtime_error("Too few arguments for reply template:  " + templateStr));
			}
			oss << args[argIndex++];
			++i; // skip 's' after '%'
		}
		else
			oss << templateStr[i];
		++i;
	}
	
	if (argIndex < args.size()) // too many arguments
	{
		throw (std::runtime_error("Too many arguments for reply template:  " + templateStr));
	}

	// return (oss.str());
	return (crlf(oss.str()));
	// return (oss.str() + "\r\n");
}

/* ************************************************************************** */


// Maybe overload the pendingPush ?
void	User::pendingPush(const std::vector<std::string>& msgs)
{
	std::vector<std::string>::const_iterator	it = msgs.begin();
	while (it != msgs.end())
	{
		pendingPush(*it);
		++it;
	}
}

// std::string getCurrentDate(void)
// {
// 	char		buffer[80];
// 	std::time_t	now = std::time(0);
// 	std::tm		*localTime = std::localtime(&now);
// 	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
	
// 	return (std::string(buffer));
// }

// to convert the '_time' attribute
std::string	Server::getServerCreationDate(void) const
{
	char	buffer[80];
	std::tm	*localTime = std::localtime(&_time);
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
	
	return (std::string(buffer));
}

void Server::user_cmd(User &client, const Message &msg)
{
	short perms = client.getPerms();
	if (perms == PERM_ALL)
		client.pendingPush(_rplGenerator.reply(462), client.getNickname());
	else if (perms == PERM_NICK)
		client.pendingPush(_rplGenerator.reply(464), client.getNickname());
	else
	{
		client.setUsername(msg.getParams());
		if (client.getPerms() == PERM_ALL)
		{
			std::string creationDate = getServerCreationDate();
			
			std::vector<std::string> welcomeReplies = _rplGenerator.generateWelcomeReplies(client.getNickname(), creationDate);
			client.pendingPush(welcomeReplies);
			// client.pendingPush(_rplGenerator.generateWelcomeReplies(client.getNickname(), creationDate));
		}
	}
}

std::vector<std::string>	Reply::generateWelcomeReplies(const std::string &nickname, const std::string &creationDate) const
{
	std::vector<std::string>	replies;

	// Generate replies
	replies.push_back(reply(RPL_WELCOME, nickname, nickname));
	replies.push_back(reply(RPL_YOURHOST, nickname, SERVER_NAME, "1.0"));
	replies.push_back(reply(RPL_CREATED, nickname, creationDate));
	replies.push_back(reply(RPL_MYINFO, nickname, SERVER_NAME, "1.0", "i t k o l"));

	return (replies);
}

// usage example:
void	test_generateWelcomeReplies(void)
{
	Reply	rpl;
	User	user(1);
	
	user.setNickname("HonoredGuest");

	Server	server("4242", "password");
	std::string	creationDate = server.getServerCreationDate();

	std::vector<std::string> welcomeReplies = rpl.generateWelcomeReplies(user.getNickname(), creationDate);
	user.pendingPush(welcomeReplies);
	
	while (user.pendingSize() > 0)
	{
		std::cout << user.pendingPop() << std::endl;
	}
}
