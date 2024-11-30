
#include "Aggregator.hpp"
#include <sstream>

Aggregator::Aggregator(void) : _buffer() {}

std::vector<std::string>	Aggregator::processData(int clientFd, const std::string &data)
{
	std::vector<std::string>	completeCommands;

	_buffer[clientFd] += data; // append incomming data to the client's buffer

	// split the buffer into commands
	std::vector<std::string>	splitCommands = _splitCommands(_buffer[clientFd]);

	// handle incomplete data in the buffer
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

#include "Aggregator.hpp"

// inside the server loop

Aggregator	aggregator;
int	clientFd = 1; // example file descriptor
std::string	incomingData = "PRIVMSG #channel :Hello\r\nPART #channel\r\n";

std::vector<std::string>	command = aggregator.processData(clientFd, incomingData);

size_t	i = 0;
while (i < commands.size())
{
	std::cout << "Command: " << commands[i] << std::endl;
	++i;
}

*/
