
#include <ctime>


// Maybe overload the pendingPush ?
void	User::pendingPush(const std::vector<std::string>& msgs)
{
	std::vector<std::string>::const_iterator	it = msgs.begin();
	while (it != msgs.end())
	{
		pendingPush(*it);
		++it;
	}
}

// to convert the '_time' attribute
std::string	Server::getServerCreationDate(void) const
{
	char	buffer[80];
	std::tm	*localTime = std::localtime(&_time);
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
	
	return (std::string(buffer));
}

void Server::user_cmd(User &client, const Message &msg)
{
	short perms = client.getPerms();
	if (perms == PERM_ALL)
		client.pendingPush(_rplGenerator.reply(462), client.getNickname());
	else if (perms == PERM_NICK)
		client.pendingPush(_rplGenerator.reply(464), client.getNickname());
	else
	{
		client.setUsername(msg.getParams());
		if (client.getPerms() == PERM_ALL)
		{
			std::string creationDate = getServerCreationDate();
			
			std::vector<std::string> welcomeReplies = _rplGenerator.generateWelcomeReplies(client.getNickname(), creationDate);
			client.pendingPush(welcomeReplies);
			// client.pendingPush(_rplGenerator.generateWelcomeReplies(client.getNickname(), creationDate));
		}
	}
}

/*
// usage example:
void	test_generateWelcomeReplies(void)
{
	User	user(1);
	user.setNickname("HonoredGuest");

	Server	server("4242", "password");
	std::string	creationDate = server.getServerCreationDate();

	std::vector<std::string> welcomeReplies = generateWelcomeReplies(user.getNickname(), creationDate);
	user.pendingPush(welcomeReplies);
	
	while (user.pendingSize() > 0)
	{
		std::cout << user.pendingPop() << std::endl;
	}
}
*/
