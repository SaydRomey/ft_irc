/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:52:54 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/28 15:59:19 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include <map>

class Message
{
	public:
		Message(const std::map<std::string, std::string> &parsedCommand);
		const std::string&	getPrefix(void) const;
		const std::string&	getCommand(void) const;
		const std::string&	getParams(void) const;
		const std::string&	getTrailing(void) const;
	
	private:
		std::string	_prefix;
		std::string	_command;
		std::string	_params;
		std::string	_trailing;
};

#endif // MESSAGE_HPP

/* TODO: handle MODE params (include first symbol in _params string)

(max 3 modes **!^)

MODE #somechannel
returns the mode of channel and
password and limit (*!! only if in the channel specified)

*/

bool _handleOperatorCommand(int clientFd, const Message &msg)
{
	
	if (msg.getCommand() == "MODE")
	{
		std::string	mode = msg.getParams;

		
		if (msg.getParams[0] == "+")
		{
			// 
		}
	}
}