
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
	CMD_UNKNOWN
};

class Validator
{
	public:
		Validator(void);
		Validator(const Validator &other);
		Validator&	operator=(const Validator &other);
		~Validator(void);
		
		bool	validateCommand(const std::map<std::string, std::string> &command) const;

		ReplyType										getRplType(void) const;
		const std::vector<std::string>					&getRplArgs(void) const;
		static const std::map<std::string, CommandType> &getCommandMap(void);
	
	private:
		typedef bool (Validator::*ValidatorFunc)(const std::map<std::string, std::string>&) const; // function pointer type for _validators
		static const ValidatorFunc	_validators[]; // internal array mapping CommandType to ValidatorFunc
		
		static const std::map<std::string, CommandType>	_commandMap;

		// error handling
		mutable ReplyType					_rplType; // store the last error code
		mutable std::vector<std::string>	_rplArgs; // store arguments for the error reply

		bool	_setRpl(ReplyType rplType, const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "") const;
		bool	_noRpl(void) const;

		bool	_validateCommandByType(CommandType cmdType, const std::map<std::string, std::string> &command) const;

		bool	_isValidNickname(const std::string &nickname) const;
		bool	_isValidChannelName(const std::string& channel) const;
		bool	_isValidModeParam(char modeFlag, const std::string &param) const;
		
		bool	_validatePassCommand(const std::map<std::string, std::string> &command) const;
		bool	_validateNickCommand(const std::map<std::string, std::string> &command) const;
		bool	_validateUserCommand(const std::map<std::string, std::string> &command) const;
		bool	_validateJoinCommand(const std::map<std::string, std::string> &command) const;
		bool	_validatePartCommand(const std::map<std::string, std::string> &command) const;
		bool	_validateTopicCommand(const std::map<std::string, std::string> &command) const;
		bool	_validateModeCommand(const std::map<std::string, std::string> &command) const;
		bool	_validateKickCommand(const std::map<std::string, std::string> &command) const;
		bool	_validateInviteCommand(const std::map<std::string, std::string> &command) const;
		bool	_validatePrivmsgCommand(const std::map<std::string, std::string> &command) const;
		bool	_validateNoticeCommand(const std::map<std::string, std::string> &command) const;

		static const size_t			MAX_NICKNAME_LENGTH;		// 9
		static const size_t			MAX_CHANNEL_NAME_LENGTH;	// 42
		static const std::string	VALID_MODE_FLAGS;			// "+-itkol"
		static const std::string	INVALID_CHANNEL_CHARS;		// " ,\r\n"
};

bool	isValidNickname(const std::string &nickname);

bool	isNickAvailable(const std::map<std::string, int> &nickMap, const std::string &nickToCheck);

#endif // VALIDATOR_HPP
