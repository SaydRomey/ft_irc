
#ifndef AGGREGATOR_HPP
# define AGGREGATOR_HPP

# include <string>
# include <map>
# include <vector>
// ...

class Aggregator
{
	public:
		Aggregator(void);
		
		std::vector<std::string>	processData(int clientFd, const std::string &data);
	
	private:
		std::map<int, std::string>	_buffer; // map each client's fd to their respective buffer

		std::vector<std::string>	_splitCommands(const std::string &raw);
}

#endif // AGGREGATOR_HPP

/*
aggregating partial commands is directly related to parsing because
it ensures that fragmented input from a client
is correctly reassembled into complete commands
before being passed to your parser.

This is crucial for handling the non-blocking nature of IRC communication,
where data may not always arrive in a single packet.

What Are Partial Commands?

In an IRC server, commands sent by clients may arrive in fragments due to:

	Network latency or low bandwidth.
	The use of TCP, which doesn't guarantee packet boundaries.
	Larger messages being split across multiple TCP packets.

For example:

A client sends the command: PRIVMSG #channel :Hello, world!\r\n

Your server may receive the data in chunks:
	First packet: PRIVMSG #cha
	Second packet: nnel :Hello, w
	Third packet: orld!\r\n
If your parser attempts to process each chunk immediately,
it will fail to interpret the complete command correctly.

*/
/*
TODO: Integrate the processData function into the server’s main loop,
where data is read from the socket.

For each client:

	Call processData(fd, incomingData) when new data arrives.
	Pass the extracted commands to the Parser.

*/
