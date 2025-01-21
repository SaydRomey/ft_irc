#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <queue>
# include <set>
# include <string>

# define PERM_PASS 0x00f
# define PERM_NICK 0x0f0
# define PERM_USER 0xf00
# define PERM_ALL  0xfff

class User
{
	private:
		int			_fd;
		std::string	_username;
		std::string	_nickname;
		short		_perms;
		bool		_closeFlag;

		std::string				_msgBuffer;
		std::queue<std::string>	_pending;
		std::set<std::string>	_channels;
	public:
		User(void);
		User(int fd);
		User& operator=(const User& other);

		void setNickname(const std::string& nickname);
		void setUsername(const std::string& username);
		void setFd(int fd);
		const std::string& getNickname(void) const;
		const std::string& getUsername(void) const;
		int getFd(void) const;

		short		getPerms(void) const;
		void		setPass(bool valid);

		void				addToMsgBuffer(const std::string& packet);
		void				resetMsgBuffer(const std::string& msg);
		const std::string	extractFromBuffer(void);

		size_t				pendingSize(void) const;
		void				pendingPush(const std::string& msg);
		const std::string	pendingPop(void);

		bool	addChannel(const std::string& chan);
		bool	removeChannel(const std::string& chan);
		bool	inChannel(const std::string& chan);

		void setCloseFlag(const std::string& reason);
		bool getCloseFlag() const;
		std::string formatPrefix() const;
};

#endif
