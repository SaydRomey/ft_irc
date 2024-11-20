/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:02:01 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/18 17:50:24 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

/*
Commands to Implement

Authentication Commands:

	PASS: Client provides the connection password.
		params: <password>
	
	NICK: Client sets their nickname.
		params: <nickname>
	
	USER: Client sets their username.
		params:		<user>	<mode>	<unused>	<realname>
		ex:	USER	guest	0		*			:Marv Forttitue

Channel Management Commands:

	JOIN: Join a specific channel.
	
	PART: Leave a specific channel.
	
	TOPIC: View or change the channel's topic.
	
	MODE: Change channel modes (i, t, k, o, l).
		params: <nickname>
	
	KICK: Eject a client from a channel.
	
	INVITE: Invite a client to a channel.

Messaging Commands:

	PRIVMSG: Send a private message to a user or channel.
	
	NOTICE: Send a notice to a user or channel.

*/

/*
[:<prefix>] <command> [<params>] [:<trailing>]


<prefix> (optional): Indicates the sender or server (e.g., :nick!user@host).

<command>: The IRC command (e.g., JOIN, PRIVMSG).

<params>: Space-separated arguments. The last parameter may include spaces if prefixed with :.

Example Messages:
	
	NICK johndoe
	JOIN #channel
	PRIVMSG #channel :Hello, everyone!

Steps to Parse and Tokenize Input

1. Read the Input String
The input is typically received over a socket as a newline-terminated string.
Strip the newline (\r\n) before processing.

2. Extract the Prefix (if present)
The prefix starts with a : and ends with the first space.

3. Identify the Command
The command is the first token after the optional prefix.

4. Tokenize Parameters
Split by spaces to extract parameters.
Treat any segment prefixed with : as the trailing parameter (it can contain spaces).

*/

# include <vector>
# include <string>

class Tokenizer
{
	public:
		Tokenizer(void);
		std::vector<std::string>	tokenize(const std::string &input);
	
	private:
		void	_split(const std::string &str, char delimiter, std::vector<std::string> &tokens);
};

/* ************************************************************************** */
# include <string>
# include <map>
# include <vector>

class CommandParser
{
	public:
		CommandParser(void);
		std::map<std::string, std::string>	parseCommand(const std::vector<std::string> &tokens);
	
	private:
		std::string	_parsePrefix(const std::string &token);
};

/* ************************************************************************** */

class MessageValidator
{
	public:
		MessageValidator(void);

	private:
		bool	_isAlphanumeric(const std::string &str);
};

/* ************************************************************************** */

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

/* ************************************************************************** */

class Parser
{
	public:
		Parser(void);
		Message	parse(const std::string &rawInput);
	
	private:
		Tokenizer			_tokenizer;
		CommandParser		_commandParser;
		MessageValidator	_validator;
};



#endif // PARSER_HPP
