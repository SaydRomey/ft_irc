/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReplyTypes.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 21:30:44 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/13 18:15:37 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_TYPES_HPP
# define REPLY_TYPES_HPP

// Enum for IRC numeric replies

enum ReplyType
{	
	// ===============
	// AUTHENTICATION
	// ===============
	RPL_WELCOME = 1,
	RPL_YOURHOST = 2,
	RPL_CREATED = 3,
	RPL_MYINFO = 4,
	
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_PASSWDMISMATCH = 464,

	// ===================
	// CHANNEL MANAGEMENT
	// ===================
	RPL_JOIN = 1000,
	RPL_PART = 1001,
	RPL_KICK = 1002,
	RPL_INVITE = 1003,
	
	RPL_CHANNELMODEIS = 324,
	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_INVITING = 341,
	RPL_NAMEREPLY = 353,
	RPL_ENDOFNAMES = 366,
	
	ERR_NOSUCHCHANNEL = 403,
	// ERR_CANNOTSENDTOCHAN = 404,
	// ERR_TOOMANYCHANNELS = 405,
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
	ERR_INVALIDMODEPARAM = 696,

	// ==========
	// MESSAGING
	// ==========
	ERR_NOSUCHNICK = 401,
	ERR_NORECIPIENT = 411,
	ERR_NOTEXTTOSEND = 412,

	// =============
	// OTHER ERRORS
	// =============
	ERR_UNKNOWNERROR = 400,
	ERR_UNKNOWNCOMMAND = 421
};

#endif // REPLY_TYPES_HPP

/* ************************************************************************** */
/* ************************************************************************** */

/*
******************
* AUTHENTICATION *
******************

*************************************
001 RPL_WELCOME
:SERVER_NAME 001 <client_nickname> :Welcome to the Internet Relay Network <client_nickname>!
:ircserv 001 nickname :Welcome to the Internet Relay Network nickname!

Sent to the client upon successful registration
to welcome them to the IRC network.

Context:
This is the first reply sent after
a successful `NICK` and `USER` command sequence,
completing the client registration.
*************************************

*************************************
002 RPL_YOURHOST
:SERVER_NAME 002 <client_nickname> :Your host is <server_name>, running version <version>
:ircserv 002 nickname :Your host is ircserv, running version 1.0

Sent to inform the client of the server’s hostname and version.

Context:
This is part of the post-authentication sequence,
sent immediately after `RPL_WELCOME`.
It provides server details.
*************************************

*************************************
003 RPL_CREATED
:SERVER_NAME 003 <client_nickname> :This server was created <creation_date>
:ircserv 003 nickname :This server was created Mon Dec 11 2024

Sent to inform the client when the server was created.

Context:
This is part of the post-authentication sequence,
sent after `RPL_YOURHOST`.
*************************************

*************************************
004 RPL_MYINFO
:SERVER_NAME 004 <client_nickname> <server_name> <version> <user_modes (empty)> <channel_modes>
:ircserv 004 nickname ircserv 1.0 - itkol

Sent to indicate basic information about the server and its configuration.

Context:
This reply is sent as part of
the post-authentication sequence after `RPL_CREATED`.
The `user_modes` field is empty (`-`) if no user modes are implemented,
and `<channel_modes>` lists available channel modes
(e.g., `itkol` for topic lock, key, operator, etc.).
*************************************

*************************************
431 ERR_NONICKNAMEGIVEN
:SERVER_NAME 431 :No nickname given
:ircserv 431 :No nickname given

Sent when the client sends a NICK command without providing a nickname.

Context:
Used in the NICK command implementation.
Example: A client sends NICK with no argument, resulting in this error.
*************************************

*************************************
432 ERR_ERRONEUSNICKNAME
:SERVER_NAME 432 <client_nickname> <nickname> :Erroneous nickname
:ircserv 432 nickname invalidnick :Erroneous nickname

Returned when a `NICK` command cannot be
successfully completed because the desired nickname
contains invalid characters or violates server rules.

Context:
This error is triggered when a client sends
a nickname that includes disallowed characters
(e.g., spaces, commas, or symbols not permitted by the server).

Example:
A client sends `NICK invalid#nick`, resulting in this error.
*************************************

*************************************
433 ERR_NICKNAMEINUSE
:SERVER_NAME 433 <client_nickname> <nickname> :Nickname is already in use
:ircserv 433 nickname user123 :Nickname is already in use

Returned when a `NICK` command cannot be successfully completed
because the desired nickname is already in use on the server.

Context:
This error is triggered when a client attempts to
register or change their nickname
to one currently being used by another client.

Example:
A client sends `NICK user123`
when another user has already taken this nickname.
*************************************

*************************************
461 ERR_NEEDMOREPARAMS
:SERVER_NAME 461 <client_nickname> <command> :Not enough parameters
:ircserv 461 nickname NICK :Not enough parameters

Returned when a client command cannot be parsed
because not enough parameters were supplied.

Context:
Relevant for all commands where parameters are mandatory.

Example:
A client sends PRIVMSG without specifying
a recipient or message, triggering this error.
*************************************

*************************************
462 ERR_ALREADYREGISTERED
:SERVER_NAME 462 <client_nickname> :You may not reregister
:ircserv 462 nickname :You may not reregister

Returned when a client attempts to re-register a detail
that can only be set during the initial registration process.

Context:
This occurs when a client tries to resend
the `PASS` or `USER` command after registration is complete.

Example:
A client sends `USER user2 0 * :New User`
after already being registered.
*************************************

*************************************
464 ERR_PASSWDMISMATCH
:SERVER_NAME 464 <client_nickname> :Password incorrect
:ircserv 464 nickname :Password incorrect

Sent when a client attempts to authenticate with
an invalid password using the PASS command.

Context:
Used during the PASS command when the provided password
does not match the server’s expected value.

Example:
A client sends PASS wrong_password and is rejected with this error.
*************************************

**********************
* CHANNEL MANAGEMENT *
**********************

*************************************
324 RPL_CHANNELMODEIS
:SERVER_NAME 324 <client_nickname> <channel> <channel_modes>
:ircserv 324 nickname #channel +itkol

Sent in response to a `MODE` command
to indicate the current modes of a channel.

Context:
This reply is sent when a client queries
or changes the modes of a channel.

`<channel_modes>` lists the active modes
(e.g., `+itkol` for topic lock, invite-only, key, etc.).

Example:
A client sends `MODE #channel`,
and the server responds with this reply
showing the channel’s active modes.
*************************************

*************************************
331 RPL_NOTOPIC
:SERVER_NAME 331 <client_nickname> <channel> :No topic is set
:ircserv 331 nickname #channel :No topic is set

Sent when a client queries a channel’s topic, but no topic is set.

Context:
This is returned in response to the `TOPIC` command
for a channel without a topic.

Example:
A client sends `TOPIC #channel`,
and the server responds with this message if no topic is set.
*************************************

*************************************
332 RPL_TOPIC
:SERVER_NAME 332 <client_nickname> <channel> :<topic>
:ircserv 332 nickname #channel :Welcome to #channel!

Sent when a client queries or sets a channel’s topic
and the channel has a topic.

Context:
This is returned in response to the `TOPIC` command
to indicate the current topic of the channel.

Example:
A client sends `TOPIC #channel`,
and the server responds with this message showing the channel’s topic.
*************************************

*************************************
341 RPL_INVITING
:SERVER_NAME 341 <client_nickname> <channel>
:ircserv 341 nickname #channel user123

Sent to confirm that a user has been invited to a channel.

Context:
This is sent in response to the `INVITE` command
to confirm the invitation.

Example:
A client sends `INVITE user123 #channel`,
and the server responds with this reply to acknowledge the invitation.
*************************************

*************************************
403 ERR_NOSUCHCHANNEL
:SERVER_NAME 403 <client_nickname> <channel> :No such channel
:ircserv 403 nickname #nonexistent :No such channel

Returned when a client attempts to interact with a non-existent channel.

Context:
This can occur when a client sends a `JOIN`, `PART`, `TOPIC`,
or other channel-specific command to a channel that does not exist.

Example:
A client sends `JOIN #nonexistent`,
and the server responds with this error.
*************************************

*************************************
404 ERR_CANNOTSENDTOCHAN
:SERVER_NAME 404 <client_nickname> <channel> :Cannot send to channel
:ircserv 404 nickname #channel :Cannot send to channel

Indicates that the `PRIVMSG` or `NOTICE` command could not be delivered to `<channel>`.

This is generally sent in response to channel modes,
such as a channel being moderated and the client
not having permission to speak on the channel,
or not being joined to a channel with the no external messages mode set.

Context:
This error is sent when:
1. The channel is moderated (`+m`),
and the client does not have permission to send messages.
2. The channel has the "no external messages" mode set (`+n`),
and the client is not a member of the channel.
3. The channel is restricted,
and the client is not authorized to send messages.

Example:
A client sends `PRIVMSG #channel :Hello`
but is not a member of a channel with the `+n` mode set.
*************************************

*************************************
405 ERR_TOOMANYCHANNELS
:SERVER_NAME 405 <client_nickname> <channel> :You have joined too many channels
:ircserv 405 nickname #channel :You have joined too many channels

Indicates that the `JOIN` command failed because
the client has reached the maximum number of channels
they are allowed to join.

Context:
This error is triggered when a client attempts to join a new channel
but is already in the maximum allowed number of channels.

Example:
A client sends `JOIN #newchannel`,
but the server’s channel limit is already reached.
*************************************

*************************************
407 ERR_TOOMANYTARGETS
:SERVER_NAME 407 <client_nickname> :Duplicate recipients. No message delivered
:ircserv 407 nickname :Duplicate recipients. No message delivered

Sent when a command specifies multiple recipients where only one is allowed,
or if there’s an ambiguity caused by duplicate or invalid targets.

Context:
This is relevant when implementing PRIVMSG or NOTICE commands,
where a message may mistakenly include multiple recipients
(e.g., comma-separated users).

Example:
A client sends PRIVMSG user1,user1 :Hello, resulting in this error.
*************************************

*************************************
441 ERR_USERNOTINCHANNEL
:SERVER_NAME 441 <client_nickname> <nick> <channel> :They aren't on that channel
:ircserv 441 nickname user123 #channel :They aren't on that channel

Returned when a client tries to perform a command
affecting a specific nickname and channel,
but the target nickname is not a member of the specified channel.

Context:
This error is triggered when commands like
`MODE`, `KICK`, or similar channel-specific actions
involve a nickname that is not in the specified channel.

Example:
A client sends `MODE #channel +o user123`,
but `user123` is not a member of `#channel`.
The server responds with this error.
*************************************

*************************************
442 ERR_NOTONCHANNEL
:SERVER_NAME 442 <client_nickname> <channel> :You're not on that channel
:ircserv 442 nickname #channel :You're not on that channel

Returned when a client attempts to perform an action
on a channel they are not a member of.

Context:
This occurs when a client sends a command
like `PART`, `TOPIC`, or `KICK`
for a channel they have not joined.

Example:
A client sends `TOPIC #channel`,
but they are not a member of the channel.
*************************************

*************************************
443 ERR_USERONCHANNEL
:SERVER_NAME 443 <client_nickname> <nick> <channel> :is already on channel
:ircserv 443 nickname user123 #channel :is already on channel

Returned when a client tries to invite a user
to a channel they are already in.

Context:
This is sent in response to the `INVITE` command
if the target user is already a member of the specified channel.

Example:
A client sends `INVITE user123 #channel`,
but `user123` is already in the channel.
*************************************

*************************************
467 ERR_KEYSET
:SERVER_NAME 467 <client_nickname> <channel> :Channel key already set
:ircserv 467 nickname #channel :Channel key already set

Sent when a client tries to set a key
(+k mode) on a channel that already has one.

Context:
Occurs when implementing the MODE command
with the +k (channel key) mode.

If the channel already has a key, attempting to set another
without first removing the existing key (-k) triggers this error.
*************************************

*************************************
471 ERR_CHANNELISFULL
:SERVER_NAME 471 <client_nickname> <channel> :Cannot join channel (+l)
:ircserv 471 nickname #channel :Cannot join channel (+l)

Returned when a client attempts to join
a channel that has reached its user limit.

Context:
This error is triggered in response to the `JOIN` command
when the channel’s user limit is exceeded.

Example:
A client sends `JOIN #channel`,
but the channel already has the maximum allowed members.
*************************************

*************************************
472 ERR_UNKNOWNMODE
:SERVER_NAME 472 <client_nickname> <char> :is unknown mode char to me
:ircserv 472 nickname x :is unknown mode char to me

Sent when a client attempts to set an invalid or
unsupported mode on a channel using the MODE command.

Context:
Occurs when implementing the MODE command.
Example:
A client sends MODE #channel +x, where x is not
a recognized channel mode, triggering this error.
*************************************

*************************************
473 ERR_INVITEONLYCHAN
:SERVER_NAME 473 <client_nickname> <channel> :Cannot join channel (+i)
:ircserv 473 nickname #channel :Cannot join channel (+i)

Returned when a client attempts to join
an invite-only channel without an invitation.

Context:
This occurs when a client sends a `JOIN` command
for a channel with the `+i` (invite-only) mode set and has not been invited.

Example:
A client sends `JOIN #channel`,
but the channel requires an invitation to join.
*************************************

*************************************
475 ERR_BADCHANNELKEY
:SERVER_NAME 475 <client_nickname> <channel> :Cannot join channel (+k)
:ircserv 475 nickname #channel :Cannot join channel (+k)

Returned to indicate that a JOIN command failed
because the channel requires a key
and the key was either incorrect or not supplied.

Not to be confused with ERR_INVALIDKEY,
which may be returned when setting a key.
*************************************

*************************************
476 ERR_BADCHANMASK
:SERVER_NAME 476 <client_nickname> <channel> :Bad channel mask
:ircserv 476 nickname #invalid :Bad channel mask

Indicates that the supplied channel name is invalid.

This is similar to, but stronger than, ERR_NOSUCHCHANNEL (403),
which indicates that the channel does not exist,
but that it may be a valid name.

Context:
This error is sent when a client tries to use a channel name that
does not match the valid naming conventions for channels
(e.g., names must start with `#` or `&` and not contain spaces or commas).

Example:
A client sends `JOIN invalid&name`
without following proper channel naming rules.
*************************************

*************************************
482 ERR_CHANOPRIVSNEEDED
:SERVER_NAME 482 <client_nickname> <channel> :You're not channel operator
:ircserv 482 nickname #channel :You're not channel operator

Returned when a client attempts to perform
a channel operator action without sufficient privileges.

Context:
This is sent when a non-operator client tries to set channel modes,
change the topic on a restricted channel, or kick users.

Example:
A client sends `MODE #channel +o user123`,
but they are not an operator of the channel.
*************************************

*************************************
696 ERR_INVALIDMODEPARAM
"<client> <target channel> <mode char> <parameter> :<description>"
:SERVER_NAME 696 <target chan/user> <mode char> <parameter> :Invalid parameter for mode
:ircserv 696 #channel k key123 :Invalid parameter for mode


Description:
Sent when a client provides an invalid parameter
for a valid mode during a MODE command.

Context:
This error is triggered when the mode itself is valid (e.g., +k, +l),
but the parameter provided does not meet the requirements for that mode.

Examples of invalid parameters:

	+k (Key Mode): The key is empty.
	+l (Limit Mode): The limit is not a positive integer.
	+o (Operator Mode): The parameter is not a valid nickname.

Examples:

Invalid Key for +k:

Client sends:
	MODE #channel +k

Server replies:
	:ircserv 696 nickname k :Invalid parameter for mode

Non-Numeric Limit for +l:

Client sends:
	MODE #channel +l not_a_number

Server replies:
	:ircserv 696 nickname l :Invalid parameter for mode

Invalid Nickname for +o:

Client sends:
	MODE #channel +o invalid!nickname

Server replies:
	:ircserv 696 nickname o :Invalid parameter for mode
*************************************

*************
* MESSAGING *
*************

*************************************
401 ERR_NOSUCHNICK
:SERVER_NAME 401 <client_nickname> <nickname> :No such nick/channel
:ircserv 401 nickname user123 :No such nick/channel

Returned when a client attempts to interact
with a non-existent nickname or channel.

Context:
This can occur when a client sends commands
like `PRIVMSG` or `INVITE`
targeting a nickname or channel that does not exist.

Example:
A client sends `PRIVMSG user123 :Hello`,
but `user123` does not exist.
*************************************

*************************************
411 ERR_NORECIPIENT
:SERVER_NAME 411 <client_nickname> :No recipient given (<command>)
:ircserv 411 nickname :No recipient given (PRIVMSG)

Returned when a client sends a command
that requires a target but does not specify one.

Context:
This error is triggered when a client sends a
`PRIVMSG` or `NOTICE` command without specifying a recipient.

Example:
A client sends `PRIVMSG :Hello`,
but no recipient is specified.
*************************************

*************************************
412 ERR_NOTEXTTOSEND
:SERVER_NAME 412 <client_nickname> :No text to send
:ircserv 412 nickname :No text to send

Returned when a client sends a command
that requires a message but does not provide one.

Context:
This error is triggered when a client sends a
`PRIVMSG` or `NOTICE` command
without specifying the message content.

Example:
A client sends `PRIVMSG user123`,
but no message is provided.
*************************************

****************
* OTHER ERRORS *
****************

*************************************
400 ERR_UNKNOWNERROR
:SERVER_NAME 400 <client_nickname> <command> :<details>
:ircserv 400 nickname NICK :An unknown error occurred
Sent as a fallback when an unspecified or unexpected error occurs,
and no other numeric reply applies.

Context:
Can be used as a generic error for unexpected conditions.
Example:
If the server encounters an internal issue while processing a command,
this error may be sent to notify the client.
*************************************

*************************************
421 ERR_UNKNOWNCOMMAND
:SERVER_NAME 421 <client_nickname> <command> :Unknown command
:ircserv 421 nickname :Unknown command
Sent to a registered client to indicate that
the command they sent isn’t known by the server.
*************************************
*/