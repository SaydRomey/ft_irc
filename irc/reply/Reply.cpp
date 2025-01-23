
#include "Reply.hpp"
#include "utils.hpp"
#include <sstream>
#include <string>

// Initialize static constant
const std::string Reply::SERVER_NAME = "ircserv";

// Singleton instance accessor
Reply&	Reply::getInstance(void)
{
	static Reply	instance;

	return (instance);
}
// Accessor for the server name (used in helper functions)
const std::string&	Reply::getServerName(void) { return (SERVER_NAME); }

/* ************************************************************************** */ // Public methods to generate replies

/*
Generates an RPL numeric reply

** use key in '_replyTemplates' map,
** use helper function 'makeArgs()' to generate variadic vector for 'args' parameter
** 'args' number must match template
*/
std::string	Reply::reply(ReplyType key, const std::vector<std::string>& args) const
{
	std::map<ReplyType, std::string>::const_iterator	it = _replyTemplates.find(key);

	if (it == _replyTemplates.end())
	{
		std::ostringstream	oss;
		oss << "Error: Unknown Reply Type (" << static_cast<int>(key) << ")";
		// throw (std::runtime_error(oss.str()));
		// std::cout << oss.str() << std::endl;
		return (oss.str());
	}
	
	return (_formatReply(it->second, args));
}

/*
Generates an RPL numeric reply
Overloaded to use an int (like 001) instead of a ReplyType (like RPL_WELCOME)
*/
std::string	Reply::reply(int key, const std::vector<std::string>& args) const
{
	return (reply(static_cast<ReplyType>(key), args));
}

/*
Generates an RPL numeric reply
Overloaded to use direct string arguments
*/
std::string	Reply::reply(ReplyType key, const std::string& arg1, const std::string& arg2, const std::string& arg3, const std::string& arg4)
{
	return (reply(key, makeArgs(arg1, arg2, arg3, arg4)));
}

/*
Generates an RPL numeric reply
Overloaded to use an int instead of a ReplyType as well as direct string arguments
*/
std::string	Reply::reply(int key, const std::string& arg1, const std::string& arg2, const std::string& arg3, const std::string& arg4)
{
	return (reply(static_cast<ReplyType>(key), arg1, arg2, arg3, arg4));
}

/* ************************************************************************** */ // Private Constructors and Destructor

Reply::Reply(void)
{
	#include "ReplyTemplates.ipp"
}

Reply::Reply(const Reply&) {}
Reply::~Reply(void) {}
Reply&	Reply::operator=(const Reply&) { return (*this); }

/* ************************************************************************** */ // Internal helper methods

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
std::string	Reply::_formatReply(const std::string& templateStr, const std::vector<std::string>& args) const
{
	std::ostringstream	oss;
	size_t				argIndex = 0;
	
	size_t	i = 0;
	while (i < templateStr.size())
	{
		// Process the template string and replace placeholders
		if (templateStr[i] == '%' && i + 1 < templateStr.size() && templateStr[i + 1] == 's')
		{
			if (argIndex >= args.size()) // too few arguments
			{
				std::ostringstream	oss;
				oss << "Error: Too few arguments for reply template:  " << templateStr << std::endl;
				return (oss.str());
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
		std::ostringstream	oss;
		oss << "Error: Too many arguments for reply template:  " << templateStr << std::endl;
		return (oss.str());
	}
	return (crlf(oss.str()));
}

/* ************************************************************************** */ // Non-member wrapper functions

/*
Generates an RPL numeric reply
*/
std::string	reply(ReplyType key, const std::vector<std::string>& args)
{
	return (Reply::getInstance().reply(key, args));
}

/*
Generates an RPL numeric reply
Overloaded to use an int (like 001) instead of a ReplyType (like RPL_WELCOME)
*/
std::string	reply(int key, const std::vector<std::string>& args)
{
	return (Reply::getInstance().reply(key, args));
}

/*
Generates an RPL numeric reply
Overloaded to use direct string arguments
*/
std::string	reply(ReplyType key, const std::string& arg1, const std::string& arg2, const std::string& arg3, const std::string& arg4)
{
	return (Reply::getInstance().reply(key, arg1, arg2, arg3, arg4));
}

/*
Generates an RPL numeric reply
Overloaded to use an int instead of a ReplyType as well as direct string arguments
*/
std::string	reply(int key, const std::string& arg1, const std::string& arg2, const std::string& arg3, const std::string& arg4)
{
	return (Reply::getInstance().reply(key, arg1, arg2, arg3, arg4));
}

/* ************************************************************************** */ // Replies 1, 2, 3 and 4 (successful authentication)

/*
Returns a vector containing the 4 replies
expected after a successful authentication

	001 RPL_WELCOME
	002 RPL_YOURHOST
	003 RPL_CREATED
	004 RPL_MYINFO
*/
std::vector<std::string>	generateWelcomeReplies(const std::string& nickname, const std::string& creationDate)
{
	Reply&						rpl = Reply::getInstance();
	std::vector<std::string>	replies;

	// Generate replies
	replies.push_back(rpl.reply(RPL_WELCOME, nickname, nickname));
	replies.push_back(rpl.reply(RPL_YOURHOST, nickname, Reply::getServerName(), "1.0"));
	replies.push_back(rpl.reply(RPL_CREATED, nickname, creationDate));
	replies.push_back(rpl.reply(RPL_MYINFO, nickname, Reply::getServerName(), "1.0", "i t k o l"));

	return (replies);
}

/* ************************************************************************** */ // Pseudo replies

/*
Generates a message when a user sends a PRIVMSG

:<senderNickname> PRIVMSG <targetNickname> :<trailingMsg>
*/
std::string	privmsgMsg(const std::string& senderNickname, const std::string& targetNickname, const std::string& trailingMsg)
{
	std::ostringstream oss;
	oss << senderNickname << " PRIVMSG " << targetNickname << " :" << trailingMsg;

	return (crlf(oss.str()));
}


/*
Generates a message for when a user joins a channel

:<clientNickname> JOIN :<channelName>
*/
std::string	joinMsg(const std::string& clientNickname, const std::string& channelName)
{
	std::ostringstream	oss;
	oss << clientNickname << " JOIN " << channelName;

	return (crlf(oss.str()));
}

/*
Generates a PART message

** If no parting message is provided, omits the trailing colon and message.

:<clientNickname> PART <channelName> [:<partingMessage>]
*/
std::string	partMsg(const std::string& clientNickname, const std::string& channelName, const std::string& partingMessage)
{
	std::ostringstream	oss;
	oss << clientNickname << " PART " << channelName;

	if (!partingMessage.empty())
		oss << " :" << partingMessage;

	return (crlf(oss.str()));
}

/*
Generates a KICK message

** If the reason is not provided, a default message is used.

:<kickerNickname> KICK <channelName> <targetNickname> [:<reason>]
*/
std::string kickMsg(const std::string& kickerNickname, const std::string& channelName, const std::string& targetNickname, const std::string& reason)
{
	std::ostringstream	oss;
	oss << kickerNickname << " KICK " << channelName << " " << targetNickname;
	
	if (!reason.empty())
		oss << " :" << reason;
	else
		oss << " :" << targetNickname << " was kicked by " << kickerNickname;

	return (crlf(oss.str()));
}

/*
Generates an INVITE message

** The channel name is always included as the trailing parameter.

:<senderNickname> INVITE <targetNickname> :<channelName>
*/
std::string	inviteMsg(const std::string& senderNickname, const std::string& targetNickname, const std::string& channelName)
{
	std::ostringstream oss;
	oss << senderNickname << " INVITE " << targetNickname << " :" << channelName;

	return (crlf(oss.str()));
}

/*
Generates a SETMODE message
*/
std::string setmodeMsg(const std::string& userNickname, const std::string& params)
{
	std::ostringstream	oss;
	oss << userNickname << " MODE " << params;

	return (crlf(oss.str()));
}
