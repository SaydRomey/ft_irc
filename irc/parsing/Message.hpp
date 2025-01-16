
#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "Parser.hpp"
# include "Reply.hpp"
# include "Validator.hpp"
# include <iostream>
# include <map>
# include <string>

typedef std::vector<std::pair<std::string, std::string> >	t_vecPairStrStr;
typedef std::map<std::string, std::string>					t_mapStrStr;
typedef std::vector<std::string>							t_vecStr;

class Message
{
	public:
		// Constructors/Destructors
		Message(const std::string &input, const std::string &nickname = "*");
		~Message(void);
		
		// Getters
		bool					isValid(void) const;
		const std::string		&getInput(void) const;
		const std::string		&getPrefix(void) const;
		const std::string		&getCommand(void) const;
		const std::string		&getParams(void) const;
		const t_vecStr			&getParamsVec(void) const;
		const std::string		&getTrailing(void) const;
		const std::string		&getReply(void) const;
		const t_vecPairStrStr	&getChannelsAndKeys(void) const;
		
	private:
		Message(void);
		Message(const Message&);
		Message& operator=(const Message&);

		void	_processInput(const std::string &input);
		
		bool			_valid;
		std::string		_nickname;
		std::string		_input;
		t_mapStrStr		_parsedMessage;
		std::string		_reply;
		t_vecPairStrStr	_channelsAndKeys;
		t_vecStr		_tokenizedParams;

		Parser			_parser;
		Validator		_validator;
};

std::ostream	&operator<<(std::ostream &out, const Message &message);

#endif // MESSAGE_HPP
