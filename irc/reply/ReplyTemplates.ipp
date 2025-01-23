
// IRC numeric replies (included in "Reply.cpp")
// Initializes template strings for numeric replies

/* ************************************************************************** */

// ===============
// AUTHENTICATION
// ===============
_replyTemplates[RPL_WELCOME] = ":" + SERVER_NAME + " 001 %s :Welcome to the Internet Relay Network %s!"; // <client_nickname> <client_nickname>
_replyTemplates[RPL_YOURHOST] = ":" + SERVER_NAME + " 002 %s :Your host is %s, running version %s"; // <client_nickname> <server_name> <version>
_replyTemplates[RPL_CREATED] = ":" + SERVER_NAME + " 003 %s :This server was created %s"; // <client_nickname> <creation_date>
_replyTemplates[RPL_MYINFO] = ":" + SERVER_NAME + " 004 %s %s %s - %s"; // <client_nickname> <server_name> <version> <channel_modes>

_replyTemplates[ERR_NONICKNAMEGIVEN] = ":" + SERVER_NAME + " 431 :No nickname given";
_replyTemplates[ERR_ERRONEUSNICKNAME] = ":" + SERVER_NAME + " 432 %s %s :Erroneous nickname"; // <client_nickname> <invalid_nickname>
_replyTemplates[ERR_NICKNAMEINUSE] = ":" + SERVER_NAME + " 433 %s %s :Nickname is already in use"; // <client_nickname> <nickname_in_use>
_replyTemplates[ERR_NOTREGISTERED] = ":" + SERVER_NAME + " 451 :You have not registered";
_replyTemplates[ERR_NEEDMOREPARAMS] = ":" + SERVER_NAME + " 461 %s %s :Not enough parameters"; // <client_nickname> <command>
_replyTemplates[ERR_ALREADYREGISTERED] = ":" + SERVER_NAME + " 462 %s :You may not reregister"; // <client_nickname>
_replyTemplates[ERR_PASSWDMISMATCH] = ":" + SERVER_NAME + " 464 %s :Password incorrect"; // <client_nickname>

// ===================
// CHANNEL MANAGEMENT
// ===================
_replyTemplates[RPL_CHANNELMODEIS] = ":" + SERVER_NAME + " 324 %s %s %s"; // <client_nickname> <channel> <channel_modes [<mode_parameters>]>
_replyTemplates[RPL_NOTOPIC] = ":" + SERVER_NAME + " 331 %s %s :No topic is set"; // <client_nickname> <channel>
_replyTemplates[RPL_TOPIC] = ":" + SERVER_NAME + " 332 %s %s :%s"; // <client_nickname> <channel> :<topic>
_replyTemplates[RPL_INVITING] = ":" + SERVER_NAME + " 341 %s %s %s"; // <client_nickname> <target_nickname> <channel>
_replyTemplates[RPL_NAMEREPLY] = ":" + SERVER_NAME + " 353 %s = %s :%s"; // <client_nickname> <channel> <user_list>
_replyTemplates[RPL_ENDOFNAMES] = ":" + SERVER_NAME + " 366 %s %s :End of /NAMES list"; // <client_nickname> <channel>

_replyTemplates[ERR_NOSUCHCHANNEL] = ":" + SERVER_NAME + " 403 %s %s :No such channel"; // <client_nickname> <channel>
_replyTemplates[ERR_TOOMANYTARGETS] = ":" + SERVER_NAME + " 407 %s :Duplicate recipients. No message delivered"; // <client_nickname>
_replyTemplates[ERR_USERNOTINCHANNEL] = ":" + SERVER_NAME + " 441 %s %s %s :They aren't on that channel"; // <client_nickname> <target_nickname> <channel>
_replyTemplates[ERR_NOTONCHANNEL] = ":" + SERVER_NAME + " 442 %s %s :You're not on that channel"; // <client_nickname> <channel>
_replyTemplates[ERR_USERONCHANNEL] = ":" + SERVER_NAME + " 443 %s %s %s :is already on channel"; // <client_nickname> <target_nickname> <channel>
_replyTemplates[ERR_KEYSET] = ":" + SERVER_NAME + " 467 %s %s :Channel key already set"; // <client_nickname> <channel>
_replyTemplates[ERR_CHANNELISFULL] = ":" + SERVER_NAME + " 471 %s %s :Cannot join channel (+l)"; // <client_nickname> <channel>
_replyTemplates[ERR_UNKNOWNMODE] = ":" + SERVER_NAME + " 472 %s %s :is unknown mode char to me"; // <client_nickname> <char>
_replyTemplates[ERR_INVITEONLYCHAN] = ":" + SERVER_NAME + " 473 %s %s :Cannot join channel (+i)"; // <client_nickname> <channel>
_replyTemplates[ERR_BADCHANNELKEY] = ":" + SERVER_NAME + " 475 %s %s :Cannot join channel (+k)"; // <client_nickname> <channel>
_replyTemplates[ERR_BADCHANMASK] = ":" + SERVER_NAME + " 476 %s %s :Bad channel mask"; // <client_nickname> <channel>
_replyTemplates[ERR_CHANOPRIVSNEEDED] = ":" + SERVER_NAME + " 482 %s %s :You're not channel operator"; // <client_nickname> <channel>
_replyTemplates[ERR_INVALIDMODEPARAM] = ":" + SERVER_NAME + " 696 %s %s %s %s :Invalid parameter for mode"; // <client_nickname> <target_channel> <mode_char> <parameter>

// ==========
// MESSAGING
// ==========
_replyTemplates[ERR_NOSUCHNICK] = ":" + SERVER_NAME + " 401 %s %s :No such nick/channel"; // <client_nickname> <nickname/channel_name>
_replyTemplates[ERR_NORECIPIENT] = ":" + SERVER_NAME + " 411 %s :No recipient given (%s)"; // <client_nickname> <command>
_replyTemplates[ERR_NOTEXTTOSEND] = ":" + SERVER_NAME + " 412 %s :No text to send"; // <client_nickname>

// ===============
// GENERAL ERRORS
// ===============
_replyTemplates[ERR_UNKNOWNERROR] = ":" + SERVER_NAME + " 400 %s %s :%s"; // <client_nickname> <command> <details>
_replyTemplates[ERR_UNKNOWNCOMMAND] = ":" + SERVER_NAME + " 421 %s %s :Unknown command"; // <client_nickname> <command>

/* ************************************************************************** */
/* ************************************************************************** */

