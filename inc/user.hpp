#ifndef USER_HPP
# define USER_HPP

# include <iostream>

class User
{
	private:
		std::string	_username;
		std::string	_nickname;
		int			_fd;
		bool		_isRegistred;
		bool		_isAuthentified;
		//à voir pour aussi mettre la liste des channels en map pour voir si il est admin ou pas. Voir si c'est pertinent car deja visible dans le channel
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
		bool isAuthentified;
		void sendMessage(const std::string& msg);
};

#endif
