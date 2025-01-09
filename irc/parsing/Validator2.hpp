
#ifndef VALIDATOR_HPP
# define VALIDATOR_HPP

# include "Message.hpp"
# include "User.hpp"
# include "Reply.hpp"
# include <map>
# include <string>
# include <vector>

typedef std::map<std::string, std::string>	t_mapStrStr;

enum CommandType
{
	PASS,
	NICK,
	USER,
	JOIN,
	PART,
	TOPIC,
	MODE,
	KICK,
	INVITE,
	PRIVMSG,
	NOTICE,
	CMD_UNKNOWN
};

class Validator
{
	public:
		Validator(void);
		~Validator(void);
		
		bool	validate(const Message &msg, const User &user) const;
	
	private:
		Validator(const Validator);
		Validator&	operator=(const Validator);
		
		typedef bool (Validator::*ValidatorFunc)(const t_mapStrStr &, const User &) const; // function pointer type for _validators
		static const std::map<CommandType, ValidatorFunc>	_validators;

		static CommandType	getCommandType(const std::string &cmd);
		
		// Validation functions
		bool	_validatePassCommand(const t_mapStrStr &command, const User &user) const;
		bool	_validateNickCommand(const t_mapStrStr &command, const User &user) const;
		bool	_validateUserCommand(const t_mapStrStr &command, const User &user) const;
		bool	_validateJoinCommand(const t_mapStrStr &command, const User &user) const;
		bool	_validatePartCommand(const t_mapStrStr &command, const User &user) const;
		bool	_validateTopicCommand(const t_mapStrStr &command, const User &user) const;
		bool	_validateModeCommand(const t_mapStrStr &command, const User &user) const;
		bool	_validateKickCommand(const t_mapStrStr &command, const User &user) const;
		bool	_validateInviteCommand(const t_mapStrStr &command, const User &user) const;
		bool	_validatePrivmsgCommand(const t_mapStrStr &command, const User &user) const;
		bool	_validateNoticeCommand(const t_mapStrStr &command, const User &user) const;

		// Helper functions
		bool	_isValidNickname(const std::string &nickname) const;
		bool	_isValidChannelName(const std::string& channel) const;
		bool	_isValidModeParam(char modeFlag, const std::string &param) const;

		static const size_t			MAX_NICKNAME_LENGTH;		// 9
		static const size_t			MAX_CHANNEL_NAME_LENGTH;	// 42
		static const std::string	VALID_MODE_FLAGS;			// "+-itkol"
		static const std::string	INVALID_CHANNEL_CHARS;		// " ,\r\n"
};

// might either make them public or in Validator_utils ? ...

bool	isValidNickname(const std::string &nickname);
bool	isNickAvailable(const std::map<std::string, int> &nickMap, const std::string &nickToCheck);

#endif // VALIDATOR_HPP
