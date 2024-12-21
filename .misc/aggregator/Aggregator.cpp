
/*
TODO: check if we put aggregator in each user object instead of server
so change map<fd, str> to just a str (for the buffer)
*/

#include "Aggregator.hpp"
#include <sstream>

const size_t	Aggregator::MAX_COMMAND_LENGTH = 512; // IRC standard max command length

Aggregator::Aggregator(void) : _buffer() {}

std::vector<std::string>	Aggregator::processData(int clientFd, const std::string &data)
{
	if (_buffer[clientFd].size() + data.size() > MAX_COMMAND_LENGTH)
	{
		throw (std::runtime_error("Command exceeds maximum allowed length")); // tmp, will handle this later (trunc?)
	}
	
	// append incomming data to the client's buffer
	_buffer[clientFd] += data;

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
	std::vector<std::string>	cmdVector;
	cmdVector.insert(cmdVector.end(), splitCommands.begin(), splitCommands.end());

	return (cmdVector);
}

/*
*/
std::vector<std::string>	Aggregator::_splitCommands(const std::string &raw)
{
	std::vector<std::string>	cmdVector;
	std::string::size_type		start = 0;
	std::string::size_type		end;

	// split based on "\r\n"
	while ((end = raw.find("\r\n", start)) != std::string::npos)
	{
		cmdVector.push_back(raw.substr(start, end - start));
		start = end + 2; // skip "\r\n"
	}

	// handle incomplete or empty commands
	if (start < raw.size())
		cmdVector.push_back(raw.substr(start));

	return (cmdVector);
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
