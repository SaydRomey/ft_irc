/* ************************************************************************** */
/* ************************************************************************** */

#include "Reply.hpp"
#include "parsing_utils.hpp"	// makeArgs()
#include <sstream>
// #include <stdexcept>
#include <string>

const std::string Reply::SERVER_NAME = "ircserv";

Reply::Reply(void)
{
	#include "ReplyTemplates.ipp" // init template reply strings
}

Reply::~Reply(void) {}

/*
Generates an RPL numeric reply
** 'args' number must match template
*/
std::string	Reply::reply(ReplyType key, const std::vector<std::string> &args) const
{
	return (_replyHelper(key, args));
}

/*
overload to use an int (like 001 instead of RPL_WELCOME)
*/
std::string	Reply::reply(int key, const std::vector<std::string> &args) const
{
	return (_replyHelper(static_cast<ReplyType>(key), args));
}

/*
overloaded reply for direct string arguments
*/
std::string	Reply::reply(ReplyType key, const std::string &arg1, const std::string &arg2, const std::string &arg3, const std::string &arg4)
{
	return (reply(key, makeArgs(arg1, arg2, arg3, arg4)));
}

/*
overload to use an int (like 001 instead of RPL_WELCOME)
*/
std::string	Reply::reply(int key, const std::string &arg1, const std::string &arg2, const std::string &arg3, const std::string &arg4)
{
	return (reply(static_cast<ReplyType>(key), makeArgs(arg1, arg2, arg3, arg4)));
}

/*
generic method to generate replies using templates
** use key in '_replyTemplates' map,
** use helper function 'makeArgs()' to generate variadic vector for 'args' parameter
*/
std::string	Reply::_replyHelper(ReplyType key, const std::vector<std::string> &args) const
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
	while (i < templateStr.size())
	{
		// process the template string and replace placeholders
		if (templateStr[i] == '%' && i + 1 < templateStr.size() && templateStr[i + 1] == 's')
		{
			if (argIndex >= args.size()) // too few arguments
			{
				std::ostringstream	oss;
				oss << "Error: Too few arguments for reply template:  " << templateStr;
				// throw (std::runtime_error(oss.str()));
				// std::cout << oss.str() << std::endl;
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
		oss << "Error: Too many arguments for reply template:  " << templateStr;
		// throw (std::runtime_error(oss.str()));
		// std::cout << oss.str() << std::endl;
		return (oss.str());
	}
	return (crlf(oss.str()));
}

/* ************************************************************************** */
/*
Static methods do not require an instance of the class to be called.

std::string response = Reply::staticReply(RPL_WELCOME, makeArgs("nickname", "nickname"));
*/
std::string	Reply::staticReply(ReplyType key, const std::vector<std::string> &args)
{
	Reply	tmpReply;
	
	return (tmpReply.reply(key, args));
}
/* ************************************************************************** */
/* ************************************************************************** */

/*
Non-member helper function (to generate a reply without needing a Reply instance)
usage:
std::string response = generateReply(RPL_WELCOME, makeArgs("nickname", "nickname"));
*/
std::string	generateReply(ReplyType key, const std::vector<std::string> &args)
{
	Reply	rpl;

	return (rpl.reply(key, args));
}

/*
*/
std::vector<std::string>	generateWelcomeReplies(const std::string &nickname, const std::string &creationDate) const
{
	Reply						rpl;
	std::vector<std::string>	replies;

	// Generate replies
	replies.push_back(rpl.reply(RPL_WELCOME, nickname, nickname));
	replies.push_back(rpl.reply(RPL_YOURHOST, nickname, SERVER_NAME, "1.0"));
	replies.push_back(rpl.reply(RPL_CREATED, nickname, creationDate));
	replies.push_back(rpl.reply(RPL_MYINFO, nickname, SERVER_NAME, "1.0", "i t k o l"));

	return (replies);
}

/*
generates a message for when a client joins a channel

:<clientNickname> JOIN :<channelName>
*/
std::string	joinMsg(const std::string &clientNickname, const std::string &channelName)
{
	std::ostringstream	oss;
	oss << ":" << clientNickname << " JOIN :" << channelName;

	return (oss.str());
}

/*
If the parting message is not provided, omits the trailing colon and message.

:<clientNickname> PART <channelName> [:<partingMessage>]
*/
std::string	partMsg(const std::string &clientNickname, const std::string &channelName, const std::string &partingMessage)
{
	std::ostringstream	oss;
	oss << ":" << clientNickname << " PART " << channelName;

	if (!partingMessage.empty())
		oss << " :" << partingMessage;

	return (oss.str());
}

/*
If the reason is not provided, a default message is used.

:<kickerNickname> KICK <channelName> <targetNickname> [:<reason>]
*/
std::string kickMsg(const std::string &kickerNickname, const std::string &channelName, const std::string &targetNickname, const std::string &reason)
{
	std::ostringstream	oss;
	oss << ":" << kickerNickname << " KICK " << channelName << " " << targetNickname;
	
	if (!reason.empty())
		oss << " :" << reason;
	else
		oss << " :" << targetNickname << " was kicked by " << kickerNickname;

	return (oss.str());
}

/*
The channel name is always included as the trailing parameter.

:<senderNickname> INVITE <targetNickname> :<channelName>
*/
std::string	inviteMsg(const std::string &senderNickname, const std::string &targetNickname, const std::string &channelName)
{
	std::ostringstream oss;
	oss << ":" << senderNickname << " INVITE " << targetNickname << " :" << channelName;

	return (oss.str());
}


/* ************************************************************************** */
// singleton Reply implementation tests
/*
Reply	Reply::_instance;

Reply&	Reply::getInstance(void)
{
	return (_instance);
}
*/
// usage
// std::string response = Reply::getInstance().reply(RPL_WELCOME, makeArgs("nickname", "nickname"));
/* ************************************************************************** */
