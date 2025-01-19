
#include <string>

/* ** Channel names are case insensitive **

Because of IRC's Scandinavian origin,
the characters {}|^ are considered to be the lower case equivalent of
the characters []\~, respectively.
This is a critical issue when determining the equivalence of
two nicknames or channel names. ***
*/
std::string	ircInsensitive(const std::string &str)
{
	if (!str || str.empty())
		return ("");

	std::string	result;
	// ...
}

/*
Default nickname generator
Looks for the next available default nickname
(or uses fd ?)
*/
std::string	generateDefaultNickname()