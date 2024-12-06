/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 02:05:32 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/06 00:18:05 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Reply.hpp"
#include <sstream>
#include <stdexcept>

const std::string Reply::SERVER_NAME = "ircserv";

Reply::Reply()
{
	#include "ReplyTemplates.ipp" // init template reply strings
}

Reply::~Reply() {}

/*
generic method to generate replies using templates
** use key in '_replyTemplates' map,
** use helper function 'makeArgs()' to generate variadic vector for 'args' parameter
*/
std::string	Reply::reply(ReplyType key, const std::vector<std::string> &args) const
{
	std::map<ReplyType, std::string>::const_iterator	it = _replyTemplates.find(key);

	if (it == _replyTemplates.end())
	{
		throw (std::runtime_error("Unknown reply key"));
	}
	
	return (_formatReply(it->second, args));
}

/*
overload to use an int (like 001 instead of RPL_WELCOME)
*/
std::string	Reply::reply(int key, const std::vector<std::string> &args) const
{
	return (reply(static_cast<ReplyType>(key), args));
}

/*
overloaded reply for direct string arguments
(check if vector one is still needed...)
*/
std::string	Reply::reply(ReplyType key, const std::string &arg1, const std::string &arg2, const std::string &arg3, const std::string &arg4)
{
	std::vector<std::string>	args;
	
	if (!arg1.empty())
		args.push_back(arg1);
	if (!arg2.empty())
		args.push_back(arg2);
	if (!arg3.empty())
		args.push_back(arg3);
	if (!arg4.empty())
		args.push_back(arg4);
	
	return (reply(key, args));
}

/*
overload to use an int (like 001 instead of RPL_WELCOME)
*/
std::string	Reply::reply(int key, const std::string &arg1, const std::string &arg2, const std::string &arg3, const std::string &arg4)
{
	return (reply(static_cast<ReplyType>(key), arg1, arg2, arg3, arg4));
}

/*
format a reply based on the template and arguments
** using ostringstream **

loop through each character in template string
replace occurrences of "%s" with corresponding argument from the vector
append non-placeholder characters directly to the result
exception thrown if mismatched placeholders and arguments..
*/
std::string	Reply::_formatReply(const std::string &templateStr, const std::vector<std::string> &args) const
{
	std::ostringstream	oss;
	size_t				argIndex = 0;
	
	size_t	i = 0;
	while (i < templateStr.size()) // process the template string and replace placeholders
	{
		if (templateStr[i] == '%' && i + 1 < templateStr.size() && templateStr[i + 1] == 's')
		{
			if (argIndex >= args.size())
				return (":ircserv 400 :Reply formatting error (too few arguments)");
				// return ("ircserv 400 :Reply formatting error (too few arguments): " + templateStr);
			
			oss << args[argIndex++];
			++i; // skip 's' after '%'
		}
		else
			oss << templateStr[i];
		++i;
	}
	if (argIndex < args.size())
		return (":ircserv 400 :Reply formatting error (too many arguments)");
		// return ("ircserv 400 :Reply formatting error (too many arguments): " + templateStr);

	return (oss.str());
}

