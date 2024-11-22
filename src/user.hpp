#ifndef USER_HPP
# define USER_HPP

# include <iostream>

class User
{
	private:
		std::string _username;
		std::string _nickname;
		//à voir pour aussi mettre la liste des channels en map pour voir si il est admin ou pas. Voir si c'est pertinent car deja visible dans le channel
	public:
		User(std::string username, std::string nickname);
		User(int fd);
		std::string getNickname();
};

#endif