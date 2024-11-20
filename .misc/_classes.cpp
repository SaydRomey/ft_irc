


/*
Writing some structural stuff to untangle my ideas
** Not an official way to organise ft_irc **

*/


/*
Represents each connected client
Stores client-specific data, such as nicknames, usernames, channels they've joined, current connection status, etc.
*/
class Client
{
	// attributes
	int	fd;
	std::string	nickname;
	std::string	username;
	std::string	realname; // optional ?
	std::vector<std::string>	channels; // channels the client has joined
	bool	isAuthenticated; // whether the client has successfully authenticated
	bool	isOperator;

	// methods
	bool	authenticate(const std::string &password);
	void	setNickname(const std::string &nickname);
	void	setUsername(const std::string &username);
	void	joinChannel(const std::string &channel); // add the channel to the client's list

};

/*
Manages individual IRC channels, including member list and channel modes
*/
class Channel
{
	// attributes
	std::string	name;
	std::string	topic;
	std::vector<Client*>	members; // list of clients in the channel
	std::vector<Client*>	operators; // list of operators in the channel
	bool	isInviteOnly;
	std::string	key; // password for the channel (optional)?
	size_t	userLimit;

	// methods
	void	addMember(Client *client);
	void	removeMember(Client *client);
	void	setTopic(const std::string &topic, Client *client); // set the topic (if client is an operator)
	bool	validateKey(const std::string &key); // validate the channel's password
};

/*
Processes commands sent by clients and executes appropriate actions
(maybe integrate a Parser object in here ?)
*/
class CommandHandler
{
	// attributes
	std::map<std::string, std::function<void(Client*, const std::vector<std::string>&)>>	commands; //maybe use function pointers instead..
	// maybe a map of command names to handler functions?

	// methods
	void	execute(Client *client, const std::string &rawCommand); // parse and execute the command (based on CommandParser's output)
	void	registerCommand(const std::string &name, std::function<void(Client*, const std::vector<std::string>&)> handler); // yep.. need to do this in a less confusing way..
	void	handleJoin(Client *client, const std::vector<std::string> &args);
	void	handlePrivmsg(Client *client, const std::vector<std::string> &args);
	void	handleKick(Client *client, const std::vector<std::string> &args);
};


/*
Coordinates clients and channels, and manages overall server operations
*/
class Server
{
	// attributes
	int	port; // listening port
	std::string	password; // server password
	std::vector<Client*>	clients; // list of connected clients
	std::map<std::string, Channel>	channels; // map of channel names to 'Channel' objects
	CommandHandler	commandHandler;

	// methods
	void	start(void); // start the server (listen for incoming connections)
	void	acceptClient(void); // accept a new client
	void	removeClient(Client *client); // remove a disconnected client
	void	sendMessage(Client *client, const std::string &message); // send a message to a specific client
	void	broadcastMessage(const std::string &channelName, const std::string &message, Client *sender); // broadcast a message to a channel
};


/*
Aggregates partial data from clients into commpplete IRC commands.
Useful for handling fragmentes messages due to low bandwidth
*/
class MessageAggregator
{
	// attributes
	std::map<int, std::string>	buffers; // maps clients file descriptor to partial command buffers

	// methods
	std::vector<std::string>	processData(innnt fd, const std::string &data); // appends data to the buffer and extract complete commands
	void	clearBuffer(int fd); // clear the buffer for a specific client
};


/*
maybe a logging or  monitoring class or functions
for debugging or monitoring server activity ?
*/
void	logInfo(const std::string &message);
void	logError(const std::string &message);



