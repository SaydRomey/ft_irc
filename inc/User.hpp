#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <queue>

class User
{
	private:
		std::string	_username;
		std::string	_nickname;
		int			_fd;
		bool		_isRegistred;
		bool		_isAuthentified;
		std::string _msgBuffer;
		std::queue<std::string> pending;
	public:
		User(void);
		User(std::string username, std::string nickname);
		User(int fd);
		User& operator=(const User& other);

		void setNickname(const std::string& nickname);
		void setUsername(const std::string& username);
		void setFd(const int& fd);
		void setRegistered(void); // Throws logic_error if nickname or username isn't set
		void setAuthentified(void);
		std::string getNickname() const;
		const std::string getUsername(void) const;
		bool isRegistered(void) const;
		bool isAuthentified(void) const;

		//for server
		void addToMsgBuffer(const std::string& packet);
		void resetMsgBuffer(const std::string& msg);
		std::string extractFromBuffer();
};

#endif
