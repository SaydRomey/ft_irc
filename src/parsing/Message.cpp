/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:57:01 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/12 15:49:19 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"
#include "parsing_utils.hpp"	// normalizeInput(), maybe trim(), tokenize()
#include "iomanip" // std::setw()

Message::Message(void) : _valid(false), _input(""), _reply("") {}
Message::~Message(void) {}

Message::Message(const Message &other)
	: _valid(false),
	  _input(other._input),
	  _parsedMessage(other._parsedMessage),
	  _reply(other._reply),
	  _channelsAndKeys(other._channelsAndKeys)
{
	// might need to implement assignable for Parser and Validator .. ?
	// _parser(other._parser),
	// _validator(other._validator)
	// *this = other;
}

Message&	Message::operator=(const Message &other)
{
	if (this != &other)
	{
		_valid = other._valid;
		_input = other._input;
		_parsedMessage = other._parsedMessage;
		_reply = other._reply;
		_channelsAndKeys = other._channelsAndKeys;
		// _parser = other._parser;
		// _validator = other._validator;
	}
	return (*this);
}

/*

** Assumes the input is a single command, as it will remove any CRLF ("\r\n") characters from it
*/
Message::Message(const std::string &input) : _valid(false), _input(input), _reply("")
{
	_processInput(normalizeInput(input));
	// _processInput(trim(normalizeInput(input)));
}

/*
	// init keys with default values
	// tokenize the input
	// parse the tokens into the map
	// merge parsedCommand into _parsedMessage, keeping defaults for missing keys
	// validate the parsed command

*/
void	Message::_processInput(const std::string &input)
{
	_parsedMessage["prefix"] = "";
	_parsedMessage["command"] = "";
	_parsedMessage["params"] = "";
	_parsedMessage["trailing"] = "";

	std::vector<std::string>	tokens = tokenize(input);
	std::map<std::string, std::string>	parsedCommand = _parser.parseCommand(tokens);

	// change for the _parser.parse(input) instead *!? (handles tokenizing) **(but fix it first...)

	std::map<std::string, std::string>::iterator	it = parsedCommand.begin();
	while (it != parsedCommand.end())
	{
		_parsedMessage[it->first] = it->second;
		++it;
	}

	Reply	rpl; // should we put it as private in Validator class ? or inside the try/if's body to not instanciate rpl if not needed
	try
	{
		if (!_validator.validateCommand(_parsedMessage))
		{
			ReplyType	errorCode = _validator.getError();
			const std::vector<std::string>	&args = _validator.getErrorArgs();
			_reply = rpl.reply(errorCode, args);
			return ; // to prevent reply format related errors from overriding _reply
		}
	}
	catch (const std::exception &e)
	{
		_reply = rpl.reply(ERR_UNKNOWNCOMMAND, "PROCESSING", "ERROR", e.what());
		_valid = false;
	}

	// (wip) if the command is "JOIN", parse channels and keys // other commands also ?
	if (_parsedMessage["command"] == "JOIN")
	{
		// assumes "params" contains exactly ? 
		if (hasMultipleEntries(_parsedMessage["params"]))
			_processChannelsAndKeys(_parsedMessage["params"]); 
		
		// ...
	}

	_valid = true;	
	_reply.clear(); // if everything is valid
}


/*
** assumes params has multiple entries
	
Creates a vector of pairs of strings, unsing ',' as a delimiter,
Pairs tokens from 'params' with tokens from 'trailing'
* Used to tokenize an input of multiple channels and keys,
formated like so:
	"#channel1,#channel2,#channel3 pass1,pass2,pass3"
or
	"#channel1,#channel2,#channel3 pass1,,pass3"

(key inputs can be empty)
*/
void Message::_processChannelsAndKeys(const std::string &params)
{
	
	// tokenize params into two parts
	std::vector<std::string>	paramTokens = tokenize(params);
	if (paramTokens.size() != 2)
		return ; // set error 400 reply ? (would already be set in validator ?)
	
	std::string	channels = paramTokens[0];
	std::string	keys = paramTokens[1];

	std::vector<std::string>	channelTokens = tokenize(channels, ',');
	std::vector<std::string>	keyTokens = tokenize(keys, ',');
	// std::vector<std::string>	channelTokens = tokenize(paramTokens[0], ',');
	// std::vector<std::string>	keyTokens = tokenize(paramTokens[0], ',')

	std::vector<std::pair<std::string, std::string> >	result;
	result.reserve(channelTokens.size()); // to avoid reallocation

	size_t	i = 0;
	while (i < channelTokens.size())
	{
		std::string	key = ""; // default to an empty key
		
		if (i < keyTokens.size())
		{
			key = keyTokens[i];
			
			if (key == "*")
			{
				key = ""; // if key is exactly "*", treat as empty field (TODO: check with teammates...)
			}
		}
		
		result.push_back(std::make_pair(channelTokens[i], key));
		++i;
	}
	_channelsAndKeys = result;
}


const std::string	&Message::getInput(void) const { return (_input); }
const std::string	&Message::getPrefix(void) const { return (_parsedMessage.at("prefix")); }
const std::string	&Message::getCommand(void) const { return (_parsedMessage.at("command")); }
const std::string	&Message::getParams(void) const { return (_parsedMessage.at("params")); }
const std::string	&Message::getTrailing(void) const { return (_parsedMessage.at("trailing")); }
const std::string	&Message::getReply(void) const { return (_reply); }

const std::vector<std::pair<std::string, std::string> >	&Message::getChannelsAndKeys(void) const
{
	return (_channelsAndKeys);
}

bool	Message::isValid(void) const
{
	return (_valid);
}

/*	TOCHECK: do we implement the channelsAndKeys conditional display?

Displays the Message object's attributes,
** Used for debug
*/
// std::ostream	&operator<<(std::ostream &oss, const Message &message)
// {
// 	const int	labelWidth = 18;
	
// 	oss << "Message Details:\n" <<
// 		GRAYTALIC << std::setw(labelWidth) << "Input:  " << RESET << message.getInput() << "\n"
// 		GRAYTALIC << std::setw(labelWidth) << "Prefix:  " << RESET << message.getPrefix() << "\n"
// 		GRAYTALIC << std::setw(labelWidth) << "Command:  " << RESET << message.getCommand() << "\n"
// 		GRAYTALIC << std::setw(labelWidth) << "Params:  " << RESET << message.getParams() << "\n"
// 		GRAYTALIC << std::setw(labelWidth) << "Trailing:  " << RESET << message.getTrailing() << "\n"
// 		GRAYTALIC << std::setw(labelWidth) << "Reply:  " << RESET << message.getReply();
	
// 	return (oss);
// }


// upgraded for multiparams channels and keys
std::ostream &operator<<(std::ostream &oss, const Message &message)
{
	const int labelWidth = 18;

	// Print basic details
	oss << "Message Details:\n"
		<< GRAYTALIC << std::setw(labelWidth) << "Input: " << RESET << message.getInput() << "\n"
		<< GRAYTALIC << std::setw(labelWidth) << "Prefix: " << RESET << message.getPrefix() << "\n"
		<< GRAYTALIC << std::setw(labelWidth) << "Command: " << RESET << message.getCommand() << "\n";

	// Tokenize params by spaces
	std::vector<std::string> paramsTokens = tokenize(message.getParams(), ' ');

	// Print params based on the number of tokens
	if (paramsTokens.size() > 1)
	{
		size_t i = 0;
		while (i < paramsTokens.size())
		{
			oss << GRAYTALIC << std::setw(labelWidth - 4) << "Param (" << (i + 1) << "): " << RESET << paramsTokens[i] << "\n";
			++i;
		}
	}
	else
	{
		oss << GRAYTALIC << std::setw(labelWidth) << "Params: " << RESET << message.getParams() << "\n";
	}

	oss << GRAYTALIC << std::setw(labelWidth) << "Trailing: " << RESET << message.getTrailing() << "\n"
		<< GRAYTALIC << std::setw(labelWidth) << "Reply: " << RESET << message.getReply() << "\n";

	// Conditionally print the channel/key pairs if the command is "JOIN" and the params are multi
	if (message.getCommand() == "JOIN" && hasMultipleEntries(message.getParams()))
	{
		oss << "\nChannels & Keys:\n";

		const std::vector<std::pair<std::string, std::string> > &pairs = message.getChannelsAndKeys();

		size_t	j = 0;
		while (j < pairs.size())
		{
			oss << GRAYTALIC << std::setw(labelWidth) << "  Channel (" << (j + 1) << "): " << RESET << pairs[j].first << "\n";

			if (!pairs[j].second.empty())
			{
				oss << GRAYTALIC << std::setw(labelWidth) << "  Key (" << (j + 1) << "): " << RESET << pairs[j].second << "\n";
			}
			else
			{
				oss << GRAYTALIC << std::setw(labelWidth) << "  Key (" << (j + 1) << "): " << RESET << "<none>" << "\n";
			}
			++j;
		}
	}

	return (oss);
}
