/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReplyTypes.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 21:30:44 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/06 02:00:40 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_TYPES_HPP
# define REPLY_TYPES_HPP

// Enum for IRC numeric replies

enum ReplyType
{
	// tmp to test, to reset the _error in Validator class
	RPL_VALID = 0,
	
	// ==========================
	// AUTHENTICATION COMMANDS
	// ==========================
	RPL_WELCOME = 1,
	RPL_YOURHOST = 2,
	RPL_CREATED = 3,
	RPL_MYINFO = 4,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,

	// ==========================
	// CHANNEL MANAGEMENT COMMANDS
	// ==========================
	RPL_CHANNELMODEIS = 324,
	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_INVITING = 341,
	ERR_NOSUCHCHANNEL = 403,
	ERR_CANNOTSENDTOCHAN = 404,
	ERR_TOOMANYCHANNELS = 405,
	ERR_TOOMANYTARGETS = 407,
	ERR_USERNOTINCHANNEL = 441,
	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,
	ERR_KEYSET = 467,
	ERR_CHANNELISFULL = 471,
	ERR_UNKNOWNMODE = 472,
	ERR_INVITEONLYCHAN = 473,
	ERR_BADCHANNELKEY = 475,
	ERR_BADCHANMASK = 476,
	ERR_CHANOPRIVSNEEDED = 482,

	// =======================
	// MESSAGING COMMANDS
	// =======================
	ERR_NOSUCHNICK = 401,
	ERR_NORECIPIENT = 411,
	ERR_NOTEXTTOSEND = 412,

	// =======================
	// OTHER ?
	// =======================
	ERR_UNKNOWNERROR = 400,
	ERR_UNKNOWNCOMMAND = 421
};

#endif // REPLY_TYPES_HPP
