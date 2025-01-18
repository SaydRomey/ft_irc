
#ifndef REPLY_HPP
# define REPLY_HPP

# include "ReplyTypes.hpp"
# include <string>
# include <map>
# include <vector>

class Reply
{
	public:
		static Reply&				getInstance(void);
		static const std::string&	getServerName(void);

		// public methods to generate replies (might remove if we only use non-member wrappers)
		std::string	reply(ReplyType key, const std::vector<std::string> &args) const;
		std::string	reply(int key, const std::vector<std::string> &args) const;
		std::string	reply(ReplyType key, const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "");
		std::string	reply(int key, const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "");

	private:
		Reply(void);
		Reply(const Reply&);
		~Reply(void);
		Reply&	operator=(const Reply&);

		// Internal helper methods
		std::string	_formatReply(const std::string &templateStr, const std::vector<std::string> &args) const;

		// internal state
		static const std::string			SERVER_NAME;
		std::map<ReplyType, std::string>	_replyTemplates;
};

// Non-member wrapper functions
std::string	reply(ReplyType key, const std::vector<std::string> &args);
std::string	reply(int key, const std::vector<std::string> &args);
std::string	reply(ReplyType key, const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "");
std::string	reply(int key, const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "");

// Replies 1, 2, 3 and 4 (successful authentication)
std::vector<std::string>	generateWelcomeReplies(const std::string &nickname, const std::string &creationDate);

// Pseudo replies
std::string	privmsgMsg(const std::string &senderNickname, const std::string &targetNickname, const std::string &trailingMsg);
std::string	joinMsg(const std::string &clientNickname, const std::string &channelName);
std::string	partMsg(const std::string &clientNickname, const std::string &channelName, const std::string &partingMessage = "");
std::string kickMsg(const std::string &kickerNickname, const std::string &channelName, const std::string &targetNickname, const std::string &reason = "");
std::string	inviteMsg(const std::string &senderNickname, const std::string &targetNickname, const std::string &channelName);
std::string setmodeMsg(const std::string &userNickname, const std::string &channelName, const std::string &modeStr);
std::string	pongMsg(const std::string &token);

#endif // REPLY_HPP
