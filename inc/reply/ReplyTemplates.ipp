// irc numeric replies (wip)
// initialize template strings for numeric replies

// ========================
// AUTHENTICATION COMMANDS
// ========================
_replyTemplates[RPL_WELCOME] = ":" + SERVER_NAME + " 001 %s :Welcome to the Internet Relay Network %s!";
_replyTemplates[RPL_YOURHOST] = ":" + SERVER_NAME + " 002 %s :Your host is %s, running version %s";
_replyTemplates[RPL_CREATED] = ":" + SERVER_NAME + " 003 %s :This server was created %s";
_replyTemplates[RPL_MYINFO] = ":" + SERVER_NAME + " 004 %s %s %s %s %s";


_replyTemplates[ERR_NONICKNAMEGIVEN] = ":" + SERVER_NAME + " 431 :No nickname given";
_replyTemplates[ERR_ERRONEUSNICKNAME] = ":" + SERVER_NAME + " 432 * %s :Erroneous nickname";
_replyTemplates[ERR_NICKNAMEINUSE] = ":" + SERVER_NAME + " 433 * %s :Nickname is already in use";
_replyTemplates[ERR_NEEDMOREPARAMS] = ":" + SERVER_NAME + " 461 %s :Not enough parameters";
_replyTemplates[ERR_ALREADYREGISTERED] = ":" + SERVER_NAME + " 462 %s:You may not reregister";

// ==========================
// CHANNEL MANAGEMENT COMMANDS
// ==========================
_replyTemplates[RPL_CHANNELMODEIS] = ":" + SERVER_NAME + " 324 %s :%s";
_replyTemplates[RPL_NOTOPIC] = ":" + SERVER_NAME + " 331 %s :No topic is set";
_replyTemplates[RPL_TOPIC] = ":" + SERVER_NAME + " 332 %s %s :%s";
_replyTemplates[RPL_INVITING] = ":" + SERVER_NAME + " 341 %s %s";

_replyTemplates[ERR_NOSUCHCHANNEL] = ":" + SERVER_NAME + " 403 %s :No such channel";
_replyTemplates[ERR_CANNOTSENDTOCHAN] = ":" + SERVER_NAME + " 404 %s :Cannot send to channel";
_replyTemplates[ERR_TOOMANYCHANNELS] = ":" + SERVER_NAME + " 405 %s :You have joined too many channels";
_replyTemplates[ERR_TOOMANYTARGETS] = ":" + SERVER_NAME + " 407 %s :Duplicate recipients. No message delivered";
_replyTemplates[ERR_USERNOTINCHANNEL] = ":" + SERVER_NAME + " 441 %s %s :They aren't on that channel";
_replyTemplates[ERR_NOTONCHANNEL] = ":" + SERVER_NAME + " 442 %s :You're not on that channel";
_replyTemplates[ERR_USERONCHANNEL] = ":" + SERVER_NAME + " 443 %s %s :is already on channel";

_replyTemplates[ERR_KEYSET] = ":" + SERVER_NAME + " 467 %s :Channel key already set";
_replyTemplates[ERR_CHANNELISFULL] = ":" + SERVER_NAME + " 471 %s :Cannot join channel (+l)";
_replyTemplates[ERR_UNKNOWNMODE] = ":" + SERVER_NAME + " 472 %s :is unknown mode char to me";
_replyTemplates[ERR_INVITEONLYCHAN] = ":" + SERVER_NAME + " 473 %s :Cannot join channel (+i)";
_replyTemplates[ERR_BADCHANNELKEY] = ":" + SERVER_NAME + " 475 %s :Cannot join channel (+k)";
_replyTemplates[ERR_BADCHANMASK] = ":" + SERVER_NAME + " 476 %s :Bad channel mask";
_replyTemplates[ERR_CHANOPRIVSNEEDED] = ":" + SERVER_NAME + " 482 %s :You're not channel operator";

// =======================
// MESSAGING COMMANDS
// =======================
_replyTemplates[ERR_NOSUCHNICK] = ":" + SERVER_NAME + " 401 %s :No such nick/channel";
_replyTemplates[ERR_NORECIPIENT] = ":" + SERVER_NAME + " 411 %s :No recipient given (%s)";
_replyTemplates[ERR_NOTEXTTOSEND] = ":" + SERVER_NAME + " 412 %s :No text to send";

// =======================
// OTHER ??
// =======================
_replyTemplates[ERR_UNKNOWNERROR] = ":" + SERVER_NAME + " 400 %s %s :%s";
_replyTemplates[ERR_UNKNOWNCOMMAND] = ":" + SERVER_NAME + " 421 %s :Unknown command.";
