/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageValidator.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:46:17 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/18 15:57:30 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_VALIDATOR_HPP
# define MESSAGE_VALIDATOR_HPP

# include <string>
# include <map>
# include <vector>

class MessageValidator
{
	public:
		MessageValidator(void);
		bool	isValidCommand(const std::map<std::string, std::string> &command);
		bool	isValidNickname(const std::string &nickname);
		bool	isValidChannel(const std::string &channel);
		
		bool	isValidTrailing(const std::string &trailing); // to test
		bool	isValidModeCommand(const std::vector<std::string> &params);
		// bool	isChannelOperatorCommand(const std::string &command);
		// bool	isMessagingCommand(const std::string &command);
		// bool	isAuthenticationCommand(const std::string &command);
		// bool	isChannelManagementCommand(const std::string &command);
	
	private:
		bool	_isAlphanumeric(const std::string &str);
};

#endif // MESSAGE_VALIDATOR_HPP
