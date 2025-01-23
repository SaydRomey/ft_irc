
#ifndef PARSING_UTILS_HPP
# define PARSING_UTILS_HPP

# define RESET		"\033[0m"
# define BOLD		"\033[1m"
# define ITALIC		"\033[3m"
# define UNDERLINE	"\033[4m"
# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define YELLOW		"\033[33m"
# define BLUE		"\033[34m"
# define PURPLE		"\033[35m"
# define CYAN		"\033[36m"
# define ORANGE		"\033[38;5;208m"
# define GRAY		"\033[90m"
# define GRAYTALIC	"\033[3;90m"

# include <algorithm>
# include <ctime>
# include <iostream>
# include <map>
# include <sstream>
# include <string>
# include <vector>
# include <utility>		// For std::pair (in parseChannelsAndKeys() and print)


typedef std::vector<std::pair<std::string, std::string> >	t_vecPairStrStr;
typedef std::map<std::string, std::string>					t_mapStrStr;
typedef std::vector<std::string>							t_vecStr;

// for the 'hasValidNumberOfParams()' function
enum VerificationType
{
	AT_LEAST,	// >=
	EXACTLY,	// ==
	AT_MOST		// <=
};

t_vecStr	makeArgs(const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "");
std::string	join(t_vecStr::const_iterator begin, t_vecStr::const_iterator end, const std::string& delim);

// Tokens
t_vecStr	tokenize(const std::string &input, char delimiter = ' ', bool includeEmptyTokens = false);
int			countTokens(const std::string &str, char delimiter = ' ');
bool		hasValidNumberOfParams(const std::string &params, VerificationType type, int expectedNum);
bool		hasMultipleEntries(const std::string &input);

// String Formatting
std::string trim(const std::string &str);
std::string	normalizeInput(const std::string &input);
std::string	crlf(const std::string &str);

// Debug
void		printVec(const t_vecStr &vec, const std::string &msg = "", bool printIndex = false);
void		printMap(const t_mapStrStr &parsedCommand, const std::string &msg = "");
void		printChannelKeyPairs(const t_vecPairStrStr &pairs, const std::string &msg = "");
void		debug_param_tokens(const std::string &params, VerificationType type, int expectedNum, int paramNum);

// Time and Date
std::string	formatTime(const time_t& time);

#endif // PARSING_UTILS_HPP
