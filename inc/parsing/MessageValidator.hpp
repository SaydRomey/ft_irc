/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageValidator.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:46:17 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/15 12:55:41 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_VALIDATOR_HPP
# define MESSAGE_VALIDATOR_HPP

# include <string>
# include <map>

class MessageValidator
{
	public:
		MessageValidator(void);
		bool	isValidCommand(const std::map<std::string, std::string> &command);
		bool	isValidNickname(const std::string &nickname);
		bool	isValidChannel(const std::string &channel);
	
	private:
		bool	_isAlphanumeric(const std::string &str);
};

#endif // MESSAGE_VALIDATOR_HPP
