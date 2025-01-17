
/*	TOCHECK:

improve nickname validator
improve username validator
improve channel name validator
	(syntax rules, limits, etc) -> uniqueness handled in higher level..
*/

#ifndef VALIDATOR_HPP
# define VALIDATOR_HPP

# include "ReplyTypes.hpp" // for numeric reply enum
# include <map>
# include <string>
# include <vector>

enum CommandType
{
	PASS = 0,
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
	PING,
	PING,
	CMD_UNKNOWN
};

typedef std::vector<std::string>			t_vecStr;
typedef std::map<std::string, int>			t_mapStrInt;
typedef std::map<std::string, std::string>	t_mapStrStr;
typedef std::map<std::string, CommandType>	t_mapStrCmdType;

class Validator
{
	public:
		Validator(void);
		~Validator(void);
		
		bool	validateCommand(const t_mapStrStr &command) const;

		ReplyType						getRplType(void) const;
		const t_vecStr					&getRplArgs(void) const;
		static const t_mapStrCmdType	&getCommandMap(void);
	
	private:
		Validator(const Validator&);
		Validator&	operator=(const Validator&);

		// function pointer type for _validators
		typedef bool	(Validator::*ValidatorFunc)(const t_mapStrStr&) const;
		
		// internal array mapping CommandType to ValidatorFunc
		static const ValidatorFunc		_validators[];
		static const t_mapStrCmdType	_commandMap;

		// error handling
		mutable ReplyType	_rplType; // store the last error code
		mutable t_vecStr	_rplArgs; // store arguments for the error reply

		bool	_setRpl(ReplyType rplType, const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "") const;
		bool	_noRpl(void) const;

		bool	_validateCommandByType(CommandType cmdType, const t_mapStrStr &command) const;

		bool	_isValidNickname(const std::string &nickname) const;
		bool	_isValidChannelName(const std::string& channel) const;
		bool	_isValidModeParam(char modeFlag, const std::string &param) const;
		
		bool	_validatePassCommand(const t_mapStrStr &command) const;
		bool	_validateNickCommand(const t_mapStrStr &command) const;
		bool	_validateUserCommand(const t_mapStrStr &command) const;
		bool	_validateJoinCommand(const t_mapStrStr &command) const;
		bool	_validatePartCommand(const t_mapStrStr &command) const;
		bool	_validateTopicCommand(const t_mapStrStr &command) const;
		bool	_validateModeCommand(const t_mapStrStr &command) const;
		bool	_validateKickCommand(const t_mapStrStr &command) const;
		bool	_validateInviteCommand(const t_mapStrStr &command) const;
		bool	_validatePrivmsgCommand(const t_mapStrStr &command) const;
		bool	_validateNoticeCommand(const t_mapStrStr &command) const;
		bool	_validatePingCommand(const t_mapStrStr &command) const;

		static const size_t			MAX_NICKNAME_LENGTH;		// 9
		static const size_t			MAX_CHANNEL_NAME_LENGTH;	// 42
		static const std::string	VALID_MODE_FLAGS;			// "+-itkol"
		static const std::string	INVALID_CHANNEL_CHARS;		// " ,\r\n"
};

bool	isValidNickname(const std::string &nickname);
bool	isNickAvailable(const t_mapStrInt &nickMap, const std::string &nickToCheck);

#endif // VALIDATOR_HPP
