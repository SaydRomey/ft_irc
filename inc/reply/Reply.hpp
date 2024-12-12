/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 01:35:46 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/12 13:16:32 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_HPP
# define REPLY_HPP

# include "ReplyTypes.hpp"
// # include "User.hpp"

# include <string>
# include <map>
# include <vector>

/*	TOCHECK:
		should we split RPL_* and ERR_* replies ?

*/
/* TODO:
	implement function to add 4 replies to a Client (class) 's pendigqeue with member function..
	
*/

class User;

class Reply
{
	public:
		Reply();
		~Reply();

		std::string	reply(ReplyType key, const std::vector<std::string> &args) const;
		std::string	reply(int key, const std::vector<std::string> &args) const;
		std::string	reply(ReplyType key, const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "");
		std::string	reply(int key, const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "");

		
	
	private:
		static const std::string	SERVER_NAME;
		
		std::map<ReplyType, std::string>	_replyTemplates;

		std::string	_formatReply(const std::string &templateStr, const std::vector<std::string> &args) const;
};

#endif // REPLY_HPP
