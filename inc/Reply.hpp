/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 01:35:46 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/29 12:41:01 by cdumais          ###   ########.fr       */
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
// const std::string SERVER_NAME = "ft_irc";
# define SERVER_NAME "ft_irc"

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

:irc.example.com 001 borja :Welcome to the Internet Relay Network borja!borja@polaris.cs.uchicago.edu

:irc.example.com 433 * borja :Nickname is already in use.

:irc.example.org 332 borja #cmsc23300 :A channel for CMSC 23300 students


*/

class Reply
{
	public:
		Reply();
		~Reply();

		std::string	generateReply(const std::string &key, const std::vector<std::string> &args);
		// std::string	generateReply(const std::string &key, const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "");
		
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
std::vector<std::string>	makeArgs(const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4);

#endif // REPLY_HPP



// Error replies
std::string Reply::needMoreParams(const std::string& command) {
    return _formatReply(_replyTemplates["ERR_NEEDMOREPARAMS"], makeArgs(command));
}

std::string Reply::nicknameInUse(const std::string& userNickname) {
    return _formatReply(_replyTemplates["ERR_NICKNAMEINUSE"], makeArgs(userNickname));
}

std::string Reply::chanOpPrivsNeeded(const std::string& channelName) {
    return _formatReply(_replyTemplates["ERR_CHANOPRIVSNEEDED"], makeArgs(channelName));
}

#include "Reply.hpp"
#include <iostream>

void test_reply() {
    Reply reply;

    try {
        std::cout << reply.welcome("HomeBoy") << std::endl;
        std::cout << reply.nicknameInUse("HomeBoy") << std::endl;
        std::cout << reply.privateMessage("Alice", "Bob", "Hello Bob!") << std::endl;
        std::cout << reply.needMoreParams("JOIN") << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
