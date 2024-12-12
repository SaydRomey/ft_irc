
#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <queue>
# include <string>

class User
{
	private:
		std::string	_username;
		std::string	_nickname;
		int			_fd;

		std::string				_msgBuffer;
		std::queue<std::string>	_pending;

		// 
		std::string	_extractedMessage; //tmp to avoid compile errors..
		std::string	_currentMessage; //tmp to avoid compile errors..
		// 
	public:
		User(void);
		User(const std::string& username, const std::string& nickname); // added const..
		User(int fd);
		User& operator=(const User& other);

		void setNickname(const std::string& nickname);
		void setUsername(const std::string& username);
		void setFd(int fd);
		std::string getNickname(void) const;
		const std::string getUsername(void) const;
		int getFd(void) const;

		//for server
		void				addToMsgBuffer(const std::string& packet);
		void				resetMsgBuffer(const std::string& msg);
		const std::string&	extractFromBuffer(void);

		size_t				pendingSize(void) const;
		void				pendingPush(const std::string& msg);
		const std::string&	pendingPop(void);
};

#endif
