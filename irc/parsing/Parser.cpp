
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
	t_mapStrStr	command;

	command["prefix"] = "";
	command["command"] = "";
	command["params"] = "";
	command["trailing"] = "";
	
	if (input.empty())
		return (command);
	
	// Tokenize input
	std::vector<std::string>	tokens = tokenize(input);
	size_t	index = 0;
	
	// Extract prefix if present (deprecated?)
	if (!tokens.empty() && tokens[index][0] == ':')
	{
		command["prefix"] = tokens[index].substr(1);
		index++;
	}
	
	// Extract command
	if (index < tokens.size() && tokens[index][0] != ':')
	{
		command["command"] = tokens[index];
		index++;
	}

	// Extract parameters and trailing
	std::string	params, trailing;
	while (index < tokens.size())
	{
		// If a colon is found, the rest is trailing
		if (tokens[index][0] == ':')
		{
			trailing = tokens[index].substr(1);
			while (++index < tokens.size())
			{
				trailing += " " + tokens[index];
			}
			break ;
		}

		// Append to params
		if (!params.empty())
			params += " ";
		params += tokens[index];
		++index;
	}

	// Assign normalized values
	command["params"] = normalizeInput(params);
	command["trailing"] = trim(trailing);

	return (command);
}

/*	
Creates a vector of pairs of strings, unsing ',' as a delimiter,
Pairs tokens from first param with tokens from second param

* Used to tokenize an input of multiple channels and keys,
formated like so:
	"#channel1,#channel2,#channel3 pass1,pass2,pass3"
or
	"#channel1,#channel2,#channel3 pass1,,pass3"

* Key inputs can be empty ('*' is considered as meaning empty)
* If there are more keys than channels, the remaining unpaired keys are ignored
*/
t_vecPairStrStr	Parser::parseChannelsAndKeys(const std::string &params) const
{
	std::vector<std::string>	paramTokens = tokenize(params);
	std::vector<std::string>	channelTokens = tokenize(paramTokens[0], ',', true);
	std::vector<std::string>	keyTokens;

	if (paramTokens.size() > 1)
		keyTokens = tokenize(paramTokens[1], ',', true);

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

// t_vecStr	Parser::parseKickParams(const std::string &params) const
// {
// 	t_vecStr	paramTokens = tokenize(params);
// 	std::string	channel = paramTokens[0];
		
// 	t_vecStr	kickParams;
// 	kickParams.push_back(channel);
		
// 	if (hasMultipleEntries(paramTokens[1]))
// 	{
// 		t_vecStr	userTokens = tokenize(paramTokens[1], ',', true);
// 		kickParams.insert(kickParams.end(), userTokens.begin(), userTokens.end());
// 	}
// 	else
// 		kickParams.push_back(paramTokens[1]);

// 	return (kickParams);
// }

/*
Extracts a vector containing the optionnal params for the MODE command
*/
t_vecStr	Parser::parseModeParams(const std::string &params) const
{
	t_vecStr	paramTokens = tokenize(params);

	if (paramTokens.empty())
		throw (std::invalid_argument("Invalid MODE command: Missing channel name"));

	// Extract mode flags if present
	std::string	modes = paramTokens.size() > 1 ? paramTokens[1] : "";
	t_vecStr	modeParamTokens(paramTokens.begin() + 2, paramTokens.end());

	// Initialize the result: [key, nickname, limit]
	t_vecStr	result(3, ""); // Default empty values for modes requiring params
	size_t		paramIndex = 0;
	size_t		i = 0;

	while (i < modes.size())
	{
		char	modeFlag = modes[i];

		// only process modes that require parameters
		if (modeFlag == 'k' || modeFlag == 'o' || modeFlag == 'l')
		{
			if (paramIndex >= modeParamTokens.size())
				continue ; // skip if no parameter is available for the flag

			const std::string &param = modeParamTokens[paramIndex];

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
