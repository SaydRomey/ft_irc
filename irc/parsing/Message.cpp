
#include "Message.hpp"
#include "parsing_utils.hpp"
#include <iomanip>

Message::Message(const std::string& input, const std::string& nickname)
	: _valid(false), _nickname(nickname), _input(input), _reply("")
{
	_processInput(input);
}

Message::~Message(void) {}

/* ************************************************************************** */ // Getters

bool					Message::isValid(void) const { return (_valid); }
const std::string&		Message::getInput(void) const { return (_input); }
const std::string&		Message::getPrefix(void) const { return (_parsedMessage.at("prefix")); }
const std::string&		Message::getCommand(void) const { return (_parsedMessage.at("command")); }
const std::string&		Message::getParams(void) const { return (_parsedMessage.at("params")); }
const t_vecStr&			Message::getParamsVec(void) const { return (_tokenizedParams); }
const std::string&		Message::getTrailing(void) const { return (_parsedMessage.at("trailing")); }
const std::string&		Message::getReply(void) const { return (_reply); }
const t_vecPairStrStr&	Message::getChannelsAndKeys(void) const { return (_channelsAndKeys); }

const std::string&		Message::getModeKey(void) const { return (_modeKey); }
const std::string&		Message::getModeNick(void) const { return (_modeNick); }
const std::string&		Message::getModeLimit(void) const { return (_modeLimit); }

/* ************************************************************************** */

/*
Focuses on the high-level flow:
parsing, validating and delegating to command-specific handlers
*/
void	Message::_processInput(const std::string& input)
{
	try
	{
		// Parse input into prefix, command, params, trailing
		_parsedMessage = _parser.parseCommand(input);

		// Override parsed prefix with User's nickname ("*" by default)
		_parsedMessage["prefix"] = _nickname;
		
		// Extract fields
		const std::string&	command = _parsedMessage["command"];
		std::string			params = _parsedMessage["params"];
		std::string			trailing = _parsedMessage["trailing"];

		// Special handling for LimeChat messages: populate trailing
		if (trailing.empty() && (command == "PRIVMSG" || command == "NOTICE" || command == "PART" || command == "TOPIC" || command == "KICK"))
		{
			size_t	requiredParamCount = (command == "KICK") ? 2 : 1;

			t_vecStr		tokenizedParams = tokenize(params);
			if (tokenizedParams.size() > requiredParamCount)
			{
				trailing = join(tokenizedParams.begin() + requiredParamCount, tokenizedParams.end(), " ");
				params = join(tokenizedParams.begin(), tokenizedParams.begin() + requiredParamCount, " ");
			}
		}

		// Handle client-specific commands and edge cases
		if (command == "PING" || command == "PONG")
		{
			_reply = (command == "PING" ? "PONG :" : "PING :") + params + trailing + "\r\n";
		}
		else if (command == "JOIN")
		{
			_channelsAndKeys = _parser.parseChannelsAndKeys(params);
		}
		else if (command == "MODE" && countTokens(params) > 2)
		{
			t_vecStr	modeParams = _parser.parseModeParams(params);

			_modeKey = modeParams[0];
			_modeNick = modeParams[1];
			_modeLimit = modeParams[2];
		}
		// else if (command == "KICK" && hasValidNumberOfParams(params, EXACTLY, 2))
		// {
		// 	_tokenizedParams = _parser.parseKickParams(params);
		// }
		// else
			// _tokenizedParams = tokenize(params);
		_tokenizedParams = tokenize(params);

		// Adjust params and trailing fields after edge cases
		_parsedMessage["params"] = params;
		_parsedMessage["trailing"] = trailing;

		// Validate the parsed command
		if (!_validator.validateCommand(_parsedMessage))
		{
			_reply = reply(_validator.getRplType(), _validator.getRplArgs());
			_valid = false;
			return ;
		}
		
		// Construct confirmation reply (wip)
		std::ostringstream	oss;
		oss << ":" << _nickname << " " << command;

		if (!params.empty())
			oss << " " << params;
		
		if (!trailing.empty())
			oss << " :" << trailing;

		_reply = crlf(oss.str());
		_valid = true;
	}
	catch (const std::exception &e)
	{
		std::string	reason = e.what();
		if (reason.empty())
			reason = "An unexpected error occured";
		
		_reply = reply(ERR_UNKNOWNERROR, _nickname, "[command]", reason);
		_valid = false;
	}
}

/* ************************************************************************** */ // << Operator overload

// Helper function to print labeled fields
static void	printLabeledField(std::ostream &oss, const std::string &label, const std::string &value, int labelWidth)
{
	oss << GRAYTALIC << std::setw(labelWidth) << label << RESET << value << "\n";
}

// Handle single parameter or multi-parameter output
static void	handleMultiParams(std::ostream &oss, const std::string &params, int labelWidth)
{
	printLabeledField(oss, "Params: ", params, labelWidth + 4);
	
	std::vector<std::string>	paramsTokens = tokenize(params, ' ');

	if (paramsTokens.size() > 1)
	{
		size_t	i = 0;
		while (i < paramsTokens.size())
		{
			oss << GRAYTALIC << std::setw(labelWidth) << "Param (" << (i) << "): " << RESET << paramsTokens[i] << "\n";
			++i;
		}
	}
}

// Handle channels and keys output
static void	handleChannelsAndKeys(std::ostream &oss, const Message &message, int labelWidth)
{
	if (message.getCommand() == "JOIN" && hasMultipleEntries(message.getParams()))
	{
		oss << "\nChannels & Keys:\n";

		const std::vector<std::pair<std::string, std::string> >	&pairs = message.getChannelsAndKeys();

		size_t	j = 0;
		while (j < pairs.size())
		{
			oss << GRAYTALIC << std::setw(labelWidth) << "  Channel (" << (j + 1) << "): " << RESET << pairs[j].first << "\n";

			if (!pairs[j].second.empty())
			{
				oss << GRAYTALIC << std::setw(labelWidth) << "  Key (" << (j + 1) << "): " << RESET << pairs[j].second << "\n\n";
			}
			else
			{
				oss << GRAYTALIC << std::setw(labelWidth) << "  Key (" << (j + 1) << "): " << RESET << "<none>" << "\n\n";
			}
			++j;
		}
	}
}

static void	handleModeParams(std::ostream &oss, const Message &message, int labelWidth)
{
	if (message.getCommand() == "MODE")
	{
		oss << "\nMode Params:\n";

		oss << GRAYTALIC << std::setw(labelWidth) << "  Mode Key: " << message.getModeKey() << RESET << "\n";
		oss << GRAYTALIC << std::setw(labelWidth) << "  Mode Nick: " << message.getModeNick() << RESET << "\n";
		oss << GRAYTALIC << std::setw(labelWidth) << "  Mode Limit: " << message.getModeLimit() << RESET << "\n";
	}
}

std::ostream	&operator<<(std::ostream &oss, const Message &message)
{
	const int	labelWidth = 18;

	// Print basic details
	oss << "Message Details:\n";
	printLabeledField(oss, "Valid: ", message.isValid() ? "True" : "False", labelWidth);
	printLabeledField(oss, "Input: ", message.getInput(), labelWidth);
	printLabeledField(oss, "Prefix: ", message.getPrefix(), labelWidth);
	printLabeledField(oss, "Command: ", message.getCommand(), labelWidth);

	// Handle multi-parameter output
	handleMultiParams(oss, message.getParams(), labelWidth - 4);

	// Print trailing and reply
	printLabeledField(oss, "Trailing: ", message.getTrailing(), labelWidth);
	printLabeledField(oss, "Reply: ", trim(message.getReply()), labelWidth);

	// Handle channels and keys
	handleChannelsAndKeys(oss, message, labelWidth);

	// Handle mode params
	handleModeParams(oss, message, labelWidth);

	return (oss);
}
