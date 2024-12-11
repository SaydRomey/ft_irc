
#ifndef IRCFILETRANSFER_HPP
#define IRCFILETRANSFER_HPP

#include <string>	// std::string
#include <fstream>	// for file stream operations

/*
The IRCFileTransfer class manages file uploads and downloads using the send and recv methods.
It handles:
	- Opening and closing files for reading and writing.
	- Reading and writing data in chunks to support large file transfers.
	- Checking the completion status of file transfers.
*/
class IRCFileTransfer
{
	private:
		std::string		fileName;	// Name of the file being transferred
		std::ifstream	inputFile;	// Input file stream for uploads
		std::ofstream	outputFile;	// Output file stream for downloads

	public:
		IRCFileTransfer(void);
		~IRCFileTransfer(void);

		bool		startUpload(const std::string &fileName);
		bool		startDownload(const std::string &fileName);
		std::string	readChunk(void);
		bool		writeChunk(const std::string &data);
		bool		isUploadComplete(void) const;
		bool		isDownloadComplete(void) const;
};

#endif // IRCFILETRANSFER_HPP
