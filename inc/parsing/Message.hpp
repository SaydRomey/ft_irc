/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:52:54 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/15 13:08:20 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include <map>

class Message
{
	public:
		Message(void);
		Message(const std::map<std::string, std::string> &parsedCommand);
		const std::string&	getPrefix(void) const;
		const std::string&	getCommand(void) const;
		const std::string&	getParams(void) const;
		const std::string&	getTrailing(void) const;

		void	setPrefix(const std::string& prefix);
		void	setCommand(const std::string& command);
		void	setParams(const std::string& params);
		void	setTrailing(const std::string& trailing);

		std::string	str(void) const;
	
	private:
		std::string	_prefix;
		std::string	_command;
		std::string	_params;
		std::string	_trailing;
};

#endif // MESSAGE_HPP
