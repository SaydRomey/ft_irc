
#include "Aggregator.hpp"
#include <sstream>

Aggregator::Aggregator(void){}

// std::vector<std::string>	Aggregator::processData(int clientFd, const std::string &data)
// {
// 	_buffer[clientFd] += data; // append incomming data to the client's buffer

// 	std::vector<std::string>	completeCommands;
// 	size_t	pos;

// 	// extract complete commands from the buffer
// 	while ((pos = _buffer[clientFd].find("\r\n")) != std::string::npos)
// 	{
// 		completeCommands.push_back(_buffer[clientFd].substr(0, pos));
// 		_buffer[clientFd].erase(0, pos + 2); // remove processed command
// 	}

// 	return (completeCommands);
// }

//or

std::vector<std::string>	Aggregator::processData(int clientFd, const std::string &data)
{
	std::vector<std::string>	completeCommands;

	_buffer[clientFd] += data; // append incomming data to the client's buffer

	// split the buffer into commands
	std::vector<std::string>	splitCommands = _splitCommands(_buffer)

	// the last element of splitCommands is the incomplete portion, if any
	if (!splitCommands.empty() && splitCommands.back().find("\r\n") == std::string::npos)
	{
		_buffer[clientFd] = splitCommands.back(); // save the incomplete portion
		splitCommands.pop_back(); // remove it from the complete commands
	}
	else
	{
		_buffer[clientFd].clear(); // clear the buffer if no incomplete data remains
	}

	// add all complete commands to the result vector
	completeCommands.insert(completeCommands.end(), splitCommands.begin(), splitCommands.end());

	return (completeCommands);
}

std::vector<std::string>	Aggregator::_splitCommands(const std::string &raw)
{
	std::vector<std::string>	commands;
	std::string::size_type	start = 0;
	std::string::size_type	end;

	// split based on "\r\n"
	while ((end = raw.find("\r\n", start)) != std::string::npos)
	{
		commands.push_back(raw.substr(start, end - start));
		start = end + 2; // skip "\r\n"
	}

	// add the remaining part (incomplete command or empty string)
	if (start < raw.size())
		commands.push_back(raw.substr(start));

	return (commands);
}

/*	example usage:

void	Server::_handleClient(int clientFd)
{
	char	buffer[1024];
	int	bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

	if (bytesRead <= 0)
	{
		_disconnectClient(clientFd, "Disconnected");
		return ;
	}

	buffer[bytesRead] = '\0';
	std::string data(buffer);

	// Aggregate and process complete commands
	std::vector<std::string>	commands = _aggregator.processData(clientFd, data);

	size_t	i = 0;
	while (i < commands.size())
	{
		try
		{
			Message message = _parser.parse(commands[i]);
			_handleCommand(clientFd, message);
		}
		catch (const std::exception &e)
		{
			std::cerr << "Error handling command: " << e.what() << std::endl;
			_sendMessage(clientFd, "ERROR: " + std::string(e.what()));
		}
		++i;
	}
}

void	handleClientData(int fd, const std::string &data)
{
	std::vector<std::string>	commands = processData(fd, data);
	
	std::vector<std::string>::const_iterator	it = commands.begin();
	while (it != commands.end())
	{
		try
		{
			Message	parsedMessage = parser.parse(*it);
			commandHandler.execute(client, parsedMessage);
		}
		catch (const std::exception &e)
		{
			// Handle parsing errors
			logError("Parsing failed for client " + std::to_string(fd) + ": " + e.what());
		}
		++it;
	}
}
*/
