/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 18:41:48 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/12 13:56:13 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_UTILS_HPP
# define PARSING_UTILS_HPP

# include <algorithm>
# include <iostream>
# include <map>
# include <sstream>
# include <string>
# include <vector>
# include <utility>		// For std::pair (in parseChannelsAndKeys() and print)
# include <stdexcept>	// std::invalid_argument (in processChannelsAndKeys)

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
# define GRAYTALIC	"\033[3;90m"

std::vector<std::string>	makeArgs(const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "");
std::vector<std::string>	tokenize(const std::string &input, char delimiter = ' ');

bool	hasMultipleEntries(const std::string &param);

// std::vector<std::pair<std::string, std::string> >	pairChannelsAndKeys(const std::string &channels, const std::string &keys);

std::string trim(const std::string &str);
std::string	normalizeInput(const std::string &input);
std::string	crlf(const std::string &str);

void	printMap(const std::map<std::string, std::string> &parsedCommand, const std::string &msg = "");
void	printChannelKeyPairs(const std::vector<std::pair<std::string, std::string> > &pairs);

#endif // PARSING_UTILS_HPP

/////



// /*
// Once the Message object is populated, higher-level components (Channel, User, etc.) can retrieve the parsed data and apply business logic.
// */
// void	ChannelManager::processJoin(const Message &message)
// {
// 	const std::vector<std::pair<std::string, std::string> >	&channelsAndKeys = message.getChannelsAndKeys();

// 	size_t i = 0;
// 	while (i < channelsAndKeys.size())
// 	{
// 		const std::string	&channel = channelsAndKeys[i].first;
// 		const std::string	&key = channelsAndKeys[i].second;

// 		if (!channelExists(channel))
// 		{
// 			createChannel(channel);
// 		}

// 		if (isValidKey(channel, key))
// 		{
// 			addUserToChannel(channel, message.getPrefix());
// 		}
// 		else
// 		{
// 			sendError(ERR_BADCHANNELKEY, channel);
// 		}
// 		++i;
// 	}
// }

