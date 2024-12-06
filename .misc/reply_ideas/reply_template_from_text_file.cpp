/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply_ideas.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 20:29:20 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/05 19:23:16 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

/*
// add private map attribute to store templates in Reply
std::map<std::string, std::string> _templates;

// call loadTemplates() in constructor
Reply::Reply()
{
	loadTemplates("ReplyTemplates.conf");
}

*/

void	Reply::loadTemplates(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		throw (std::runtime_error("Failed to open templates file: " + filename));
	}

	std::string line;
	size_t lineNumber = 0;

	while (std::getline(file, line))
	{
		++lineNumber;

		// Trim whitespace from both ends
		size_t start = line.find_first_not_of(" \t");
		size_t end = line.find_last_not_of(" \t");
		if (start == std::string::npos || line[start] == '#')
		{
			// Skip empty lines or lines that start with a comment
			continue ;
		}
		line = line.substr(start, end - start + 1);

		// Find the separator '='
		size_t separator = line.find('=');
		if (separator == std::string::npos)
		{
			std::ostringstream oss;
			oss << "Invalid format at line " << lineNumber << ": missing '=' separator";
			throw (std::runtime_error(oss.str()));
		}

		// Parse the key and value
		std::string key = line.substr(0, separator);
		std::string value = line.substr(separator + 1);

		// Insert into the map
		if (_templates.find(key) != _templates.end())
		{
			std::ostringstream oss;
			oss << "Duplicate key at line " << lineNumber << ": " << key;
			throw (std::runtime_error(oss.str()));
		}
		_templates[key] = value;
	}

	file.close();
}

/*
(simplified version, does not allow empty lines or comments, no error checking for duplicates, bad format (key=value) or leading/trailing whitespace..)
*/
// void Reply::loadTemplates(const std::string &filename)
// {
// 	std::ifstream file(filename.c_str());
// 	if (!file.is_open())
// 	{
// 		throw (std::runtime_error("Failed to open templates file: " + filename));
// 	}

// 	std::string line;
// 	while (std::getline(file, line))
// 	{
// 		size_t separator = line.find('=');
// 		if (separator != std::string::npos)
// 		{
// 			std::string key = line.substr(0, separator);
// 			std::string value = line.substr(separator + 1);
// 			_templates[key] = value;
// 		}
// 	}
// }



// ReplyTemplate file:

/*
# ========================
# AUTHENTICATION COMMANDS
# ========================

# Sent upon successful connection to the server
RPL_WELCOME=:ircserv 001 %s :Welcome to the Internet Relay Network %s!

# Sent after successful connection, detailing the server info
RPL_YOURHOST=:ircserv 002 %s :Your host is %s, running version %s
RPL_CREATED=:ircserv 003 %s :This server was created %s
RPL_MYINFO=:ircserv 004 %s %s %s %s %s

# Error replies for nickname and authentication issues
ERR_NONICKNAMEGIVEN=:ircserv 431 :No nickname given
ERR_ERRONEUSNICKNAME=:ircserv 432 * %s :Erroneous nickname
ERR_NICKNAMEINUSE=:ircserv 433 * %s :Nickname is already in use
ERR_NEEDMOREPARAMS=:ircserv 461 %s :Not enough parameters
ERR_ALREADYREGISTERED=:ircserv 462 :You may not reregister


# ==========================
# CHANNEL MANAGEMENT COMMANDS
# ==========================

# Replies related to channel modes
RPL_CHANNELMODEIS=:ircserv 324 %s :%s

# Topic-related replies
RPL_NOTOPIC=:ircserv 331 %s :No topic is set
RPL_TOPIC=:ircserv 332 %s %s :%s

# Invite-related replies
RPL_INVITING=:ircserv 341 %s %s

# Error replies for channel issues
ERR_NOSUCHCHANNEL=:ircserv 403 %s :No such channel
ERR_CANNOTSENDTOCHAN=:ircserv 404 %s :Cannot send to channel
ERR_TOOMANYCHANNELS=:ircserv 405 %s :You have joined too many channels
ERR_TOOMANYTARGETS=:ircserv 407 %s :Duplicate recipients. No message delivered
ERR_USERNOTINCHANNEL=:ircserv 441 %s %s :They aren't on that channel
ERR_NOTONCHANNEL=:ircserv 442 %s :You're not on that channel
ERR_USERONCHANNEL=:ircserv 443 %s %s :is already on channel

# Replies for channel key and mode issues
ERR_KEYSET=:ircserv 467 %s :Channel key already set
ERR_CHANNELISFULL=:ircserv 471 %s :Cannot join channel (+l)
ERR_UNKNOWNMODE=:ircserv 472 %s :is unknown mode char to me
ERR_INVITEONLYCHAN=:ircserv 473 %s :Cannot join channel (+i)
ERR_BADCHANNELKEY=:ircserv 475 %s :Cannot join channel (+k)
ERR_BADCHANMASK=:ircserv 476 %s :Bad channel mask
ERR_CHANOPRIVSNEEDED=:ircserv 482 %s :You're not channel operator


# =======================
# MESSAGING COMMANDS
# =======================

# Error replies for messaging issues
ERR_NOSUCHNICK=:ircserv 401 %s :No such nick/channel
ERR_CANNOTSENDTOCHAN=:ircserv 404 %s :Cannot send to channel
ERR_TOOMANYTARGETS=:ircserv 407 %s :Duplicate recipients. No message delivered
ERR_NORECIPIENT=:ircserv 411 :No recipient given (%s)
ERR_NOTEXTTOSEND=:ircserv 412 :No text to send
*/
