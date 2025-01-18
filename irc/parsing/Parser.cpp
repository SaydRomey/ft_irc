
#include "Parser.hpp"
#include "parsing_utils.hpp"
#include "Reply.hpp"
// #include <stdexcept>
#include <utility>				// std::make_pair()

Parser::Parser(void) {}
Parser::~Parser(void) {}

/*
Extracts and organize command data
	** (not anymore ?) ** prefix if first token starts with ':'
	command
	parameters
	trailing message if ':' is found after the command
*/
t_mapStrStr	Parser::parseCommand(const std::string &input) const
{
	t_mapStrStr					command;
	std::vector<std::string>	tokens = tokenize(input);

	command["prefix"] = "";
	command["command"] = "";
	command["params"] = "";
	command["trailing"] = "";
	
	if (tokens.empty())
		return (command);
	
	size_t	index = 0;
	
	// extract prefix if present
	if (tokens[index][0] == ':')
	{
		command["prefix"] = tokens[index].substr(1);
		index++;
	}
	
	// extract command
	if (index < tokens.size() && tokens[index][0] != ':')
	{
		command["command"] = tokens[index];
		index++;
	}

	// extract parameters and trailing
	std::string	params;
	while (index < tokens.size())
	{
		if (tokens[index][0] == ':')
		{
			// everything after ':' is the trailing message
			std::string	trailing = tokens[index].substr(1);
			while (++index < tokens.size())
			{
				trailing += " " + tokens[index];
			}
			command["trailing"] = trim(trailing);
			break ;
		}
		if (!params.empty())
			params += " ";
		params += tokens[index];
		++index;
	}
	command["params"] = normalizeInput(params);

	return (command);
}

/*	**need to decide if we skip or flag a missing channel in multiparams "#chan1,,#chan3,#chan4"

** assumes params has multiple entries and has at most 2 params
** (checked in Message::_processJoinCommand())
	
Creates a vector of pairs of strings, unsing ',' as a delimiter,
Pairs tokens from first param with tokens from second param

* Used to tokenize an input of multiple channels and keys,
formated like so:
	"#channel1,#channel2,#channel3 pass1,pass2,pass3"
or
	"#channel1,#channel2,#channel3 pass1,,pass3"

* Key inputs can be empty ('*' is considered as meaning empty)
* If more keys than channels, the remaining unpaired keys are ignored
*/
t_vecPairStrStr	Parser::parseChannelsAndKeys(const std::string &params) const
{
	std::vector<std::string>	paramTokens = tokenize(params);
	std::vector<std::string>	channelTokens = tokenize(paramTokens[0], ',', true);
	std::vector<std::string>	keyTokens = tokenize(paramTokens[1], ',', true);

	t_vecPairStrStr	result;
	result.reserve(channelTokens.size());

	size_t	i = 0;
	while (i < channelTokens.size())
	{
		std::string	key = "";
		
		if (i < keyTokens.size() && keyTokens[i] != "*")
		{
			key = keyTokens[i];
		}
		
		result.push_back(std::make_pair(channelTokens[i], key));
		++i;
	}
	return (result);
}

t_vecStr	Parser::parseKickParams(const std::string &params) const
{
	t_vecStr	paramTokens = tokenize(params);
	std::string	channel = paramTokens[0];
		
	t_vecStr	kickParams;
	kickParams.push_back(channel);
		
	if (hasMultipleEntries(paramTokens[1]))
	{
		t_vecStr	userTokens = tokenize(paramTokens[1], ',', true);
		kickParams.insert(kickParams.end(), userTokens.begin(), userTokens.end());
	}
	else
		kickParams.push_back(paramTokens[1]);

	return (kickParams);
}

t_vecStr	Parser::parseModeParams(const std::string &modes, const t_vecStr &paramTokens) const
{
	t_vecStr	result(3, ""); // [key, nickname, limit]
	size_t		paramIndex = 0;

	size_t	i = 0;
	while (i < modes.size())
	{
		char	modeFlag = modes[i];

		// only process modes that require parameters
		if (modeFlag == 'k' || modeFlag == 'o' || modeFlag == 'l')
		{
			const std::string &param = paramTokens[paramIndex];

			// assign parameter to the appropriate index
			switch (modeFlag)
			{
				case 'k':
					result[0] = param; // key
					break ;
				case 'o':
					result[1] = param; // nickname
					break ;
				case 'l':
					result[2] = param; // limit
					break ;
			}
			++paramIndex;
		}
		++i;
	}
	return (result);
}

// t_vecStr	Parser::parseModeParams(const std::string &params) const
// {
// 	t_vecStr	paramTokens = tokenize(params);
// 	std::string	modes = paramTokens[1];
// 	t_vecStr	modeParamTokens(paramTokens.begin() + 2, paramTokens.end());

// 	t_vecStr	result(3, ""); // [key, nickname, limit]
// 	size_t		paramIndex = 0;

// 	size_t	i = 0;
// 	while (i < modes.size())
// 	{
// 		char	modeFlag = modes[i];

// 		// only process modes that require parameters
// 		if (modeFlag == 'k' || modeFlag == 'o' || modeFlag == 'l')
// 		{
// 			const std::string &param = modeParamTokens[paramIndex];

// 			// assign parameter to the appropriate index
// 			switch (modeFlag)
// 			{
// 				case 'k':
// 					result[0] = param; // key
// 					break ;
// 				case 'o':
// 					result[1] = param; // nickname
// 					break ;
// 				case 'l':
// 					result[2] = param; // limit
// 					break ;
// 			}
// 			++paramIndex;
// 		}
// 		++i;
// 	}
// 	return (result);
// }
