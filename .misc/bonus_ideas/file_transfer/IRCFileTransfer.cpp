

#include "IRCFileTransfer.hpp"

/*
Constructs an IRCFileTransfer instance.
*/
IRCFileTransfer::IRCFileTransfer(void) {}

/*
Destroys the IRCFileTransfer instance.
Closes any open file streams to ensure no resource leaks.
*/
IRCFileTransfer::~IRCFileTransfer(void)
{
	// Close the input file stream if it's open
	if (inputFile.is_open())
		inputFile.close();

	// Close the output file stream if it's open
	if (outputFile.is_open())
		outputFile.close();
}

/*
Starts a file upload (reading from the server to the client).
Parameters:
	fileName - The name of the file to be uploaded.
Returns:
	true if the file was successfully opened for reading, false otherwise.
*/
bool	IRCFileTransfer::startUpload(const std::string &fileName)
{
	// Store the file name and open the file for reading in binary mode
	this->fileName = fileName;
	inputFile.open(fileName.c_str(), std::ios::binary);

	// Return true if the file was successfully opened
	return (inputFile.is_open());
}

/*
Starts a file download (writing from the client to the server).
Parameters:
	fileName - The name of the file to be downloaded.
Returns:
	true if the file was successfully opened for writing, false otherwise.
*/
bool	IRCFileTransfer::startDownload(const std::string &fileName)
{
	// Store the file name and open the file for writing in binary mode
	this->fileName = fileName;
	outputFile.open(fileName.c_str(), std::ios::binary);

	// Return true if the file was successfully opened
	return (outputFile.is_open());
}

/*
Reads a chunk of data from the input file.
Returns:
	A string containing the chunk of data read from the file.
	Returns an empty string if the file is not open or EOF is reached.
*/
std::string	IRCFileTransfer::readChunk(void)
{
	// Return an empty string if the input file is not open
	if (!inputFile.is_open())
		return ("");

	char buffer[1024]; // Buffer to store the chunk of data

	// Read a chunk of data from the file
	inputFile.read(buffer, sizeof(buffer));

	// Return the data read as a string, truncating to the actual number of bytes read
	return (std::string(buffer, inputFile.gcount()));
}

/*
Writes a chunk of data to the output file.
Parameters:
	data - The chunk of data to write to the file.
Returns:
	true if the data was successfully written, false otherwise.
*/
bool	IRCFileTransfer::writeChunk(const std::string &data)
{
	// Return false if the output file is not open
	if (!outputFile.is_open())
		return (false);

	// Write the data to the file
	outputFile.write(data.c_str(), data.size());

	// Return true if the write operation was successful
	return (outputFile.good());
}

/*
Checks if the upload (reading) process is complete.
Returns:
	true if the end of the file has been reached, false otherwise.
*/
bool	IRCFileTransfer::isUploadComplete(void) const
{
	// Check if the end of the file has been reached
	return (inputFile.eof());
}

/*
Checks if the download (writing) process is complete.
Returns:
	true if the file is still in a good state for writing, false otherwise.
*/
bool	IRCFileTransfer::isDownloadComplete(void) const
{
	// Check if the output file is still in a good state
	return (outputFile.good());
}
