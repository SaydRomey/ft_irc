
#include "Message.hpp"
#include "parsing_utils.hpp"	// normalizeInput(), maybe trim(), tokenize()
#include <iomanip>				// std::setw()

Message::Message(const std::string &input, const std::string &nickname)
	: _valid(false), _nickname(nickname), _input(input), _reply("")
{
	_processInput(input);
	// _processInput(normalizeInput(input));
	// _processInput(trim(normalizeInput(input)));
}


Message::~Message(void) {}

/* ************************************************************************** */

bool					Message::isValid(void) const { return (_valid); }
const std::string		&Message::getInput(void) const { return (_input); }
const std::string		&Message::getPrefix(void) const { return (_parsedMessage.at("prefix")); }
const std::string		&Message::getCommand(void) const { return (_parsedMessage.at("command")); }
const std::string		&Message::getParams(void) const { return (_parsedMessage.at("params")); }
const t_vecStr			&Message::getParamsVec(void) const { return (_tokenizedParams); }
const std::string		&Message::getTrailing(void) const { return (_parsedMessage.at("trailing")); }
const std::string		&Message::getReply(void) const { return (_reply); } // replies have a "\r\n" appended to them
const t_vecPairStrStr	&Message::getChannelsAndKeys(void) const { return (_channelsAndKeys); }

/* ************************************************************************** */

/*
Focuses on the high-level flow:
parsing, validating and delegating to command-specific handlers
*/
void	Message::_processInput(const std::string &input)
{
	try
	{
		_parsedMessage = _parser.parseCommand(input);

		// assign default prefix if not provided
		// if (_parsedMessage["prefix"].empty())
		// 	_parsedMessage["prefix"] = _nickname;

		// override parsed prefix with User's nickname ("*" by default)
		_parsedMessage["prefix"] = _nickname;
		
		// validate the parsed command
		if (!_validator.validateCommand(_parsedMessage))
		{
			_reply = reply(_validator.getRplType(), _validator.getRplArgs());
			_valid = false;
			return ;
		}

		// extract command and params
		const std::string &command = _parsedMessage["command"];

		// t_mapStrStr::iterator	commandIt = _parsedMessage.find("command");
		// if (commandIt == _parsedMessage.end())
		// {
		// 	_reply = reply(ERR_UNKNOWNCOMMAND, "*", "*Missing command*");
		// 	_valid = false;
		// 	return ;
		// }
		// const std::string	&command = commandIt->second;

		


		t_mapStrStr::iterator	paramsIt = _parsedMessage.find("params");
		const std::string	params = (paramsIt != _parsedMessage.end()) ? paramsIt->second : "";

		// dispatch to command-specific handlers
		if (command == "JOIN" && countTokens(paramsIt->second) > 1)
		{
			if (hasValidNumberOfParams(params, AT_MOST, 2))
				_channelsAndKeys = _parser.parseChannelsAndKeys(params);
		}
		else if (command == "KICK" && hasValidNumberOfParams(paramsIt->second, EXACTLY, 2))
			_tokenizedParams = _parser.parseKickParams(params);
		else
		{
			_tokenizedParams = tokenize(params);
		}
		
		_valid = true;
		_reply.clear();
	}
	catch (const std::exception &e)
	{
		std::string	reason = e.what();
		if (reason.empty())
			reason = "EMPTY REASON";
		
		_reply = reply(ERR_UNKNOWNERROR, "*", "[command]", reason);
		_valid = false;
	}
}

/* ************************************************************************** */ // Operator overload

// Helper function to print labeled fields
static void	printLabeledField(std::ostream &oss, const std::string &label, const std::string &value, int labelWidth)
{
	oss << GRAYTALIC << std::setw(labelWidth) << label << RESET << value << "\n";
}

// Handle single parameter or multi-parameter output
static void	handleMultiParams(std::ostream &oss, const std::string &params, int labelWidth)
{
	std::vector<std::string>	paramsTokens = tokenize(params, ' ');

	if (paramsTokens.size() > 1)
	{
		size_t	i = 0;
		while (i < paramsTokens.size())
		{
			oss << GRAYTALIC << std::setw(labelWidth) << "Param (" << (i + 1) << "): " << RESET << paramsTokens[i] << "\n";
			++i;
		}
	}
	else
	{
		printLabeledField(oss, "Params: ", params, labelWidth + 4);
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

	return (oss);
}
