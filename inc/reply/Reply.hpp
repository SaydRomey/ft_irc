
#ifndef REPLY_HPP
# define REPLY_HPP

# include "ReplyTypes.hpp"

# include <string>
# include <map>
# include <vector>

class Reply
{
	public:
		Reply(void);
		~Reply(void);

		std::string	reply(ReplyType key, const std::vector<std::string> &args) const;
		std::string	reply(int key, const std::vector<std::string> &args) const;
		std::string	reply(ReplyType key, const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "");
		std::string	reply(int key, const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "");

		static std::string	staticReply(ReplyType key, const std::vector<std::string> &args);

	private:
		static const std::string	SERVER_NAME;
		
		std::map<ReplyType, std::string>	_replyTemplates;

		std::string	_replyHelper(ReplyType key, const std::vector<std::string> &args) const;
		std::string	_formatReply(const std::string &templateStr, const std::vector<std::string> &args) const;
};

// non-member helper function (that use a Reply object)
std::string	generateReply(ReplyType key, const std::vector<std::string> &args);
std::vector<std::string>	generateWelcomeReplies(const std::string &nickname, const std::string &creationDate);

// pseudo replies
std::string	joinMsg(const std::string &clientNickname, const std::string &channelName);
std::string	partMsg(const std::string &clientNickname, const std::string &channelName, const std::string &partingMessage = "");
std::string kickMsg(const std::string &kickerNickname, const std::string &channelName, const std::string &targetNickname, const std::string &reason = "");
std::string	inviteMsg(const std::string &senderNickname, const std::string &targetNickname, const std::string &channelName);

// singleton Reply implementation tests
/*
class Reply
{
	public:
		static Reply&	getInstance(void); // singleton accessor
		std::string		reply(ReplyType key, const std::vector<std::string> &args) const;
	
	private:
		Reply();
		static Reply	_instance; // the single instance
}
*/

#endif // REPLY_HPP
