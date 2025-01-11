
#include "Message.hpp"
#include "Reply.hpp"
#include "_test_header.hpp"
#include <iomanip>
#include <iostream>

static size_t	getPrintableLength(const std::string &str)
{
	size_t	length = 0;
	bool	inEscape = false;
	
	std::string::const_iterator it = str.begin();
	while (it != str.end())
	{
		if (*it == '\033')
			inEscape = true;
		else if (inEscape && *it == 'm')
			inEscape = false;
		else if (!inEscape)
			++length;
		++it;
	}
	return (length);
}

static void	printBox(const std::string &text, char borderChar = '*', const std::string &color = "", bool newLines = true)
{
	size_t		textLength = getPrintableLength(text);
	size_t		boxWidth = textLength + 4; // 2 spaces + 2 border chars

	std::string	borderLine(boxWidth, borderChar);
	std::string newLine = (newLines) ? "\n" : "";

	std::cout << newLine << color << borderLine << "\n";

	std::cout << borderChar << RESET << " ";
	std::cout << text << RESET;
	std::cout << " " << color << borderChar << "\n";

	std::cout << borderLine << RESET << newLine << std::endl;
}

/*
Formats the text for a box with optional bold and italic style
** Will format the string as "Testing <testType> Messages"
*/
static std::string	formatBoxText(const std::string &testType, const std::string &typeColor, bool bold = false, bool italic = false)
{
	std::string	formattedText;

	if (bold)
		formattedText += BOLD;

	formattedText += "Testing";
	
	if (italic)
		formattedText += ITALIC;

	formattedText += " " + typeColor + testType + RESET;

	if (bold)
		formattedText += BOLD;

	formattedText += " Messages" + std::string(RESET);

	return (formattedText);
}

static void	printInput(const std::string &input, bool prefixNewLine = false)
{
	std::cout << GRAYTALIC \
	<< (prefixNewLine ? "\n" : "") \
	<< "\"" << input << "\"" << RESET << std::endl;
}

static void printMsg(const Message &message, size_t i, const std::string &testType, bool success)
{
	std::string	result;
	
	if (success)
		result = std::string(std::string(GREEN) + "SUCCESS!");
	else
		result = std::string(std::string(RED) + "FAILURE!");

	printInput(message.getInput());
	std::cout << testType << " Message Test " << (i + 1) << ": " << result << RESET << std::endl;

	if (success)
		std::cout << message << std::endl;
	else
		std::cout << "  Reply: " << message.getReply() << std::endl;
}

void	runTests(const std::string messages[], size_t count, const std::string &testType, bool expectSuccess)
{
	std::string	expectColor = (expectSuccess) ? YELLOW : ORANGE;
	std::string	typeColor = (testType == "Valid") ? GREEN : RED;

	std::string	boxText = formatBoxText(testType, typeColor);
	printBox(boxText, '*', expectColor);

	size_t	i = 0;
	while (i < count)
	{
		try
		{
			Message	msg(messages[i]);
			bool	success = msg.getReply().empty() == expectSuccess; // comparing reply state with expected result
			// bool	success = msg.isValid() == expectSuccess; // comparing Message '_valid' attribute with expected result (to test)
			printMsg(msg, i, testType, success);
		}
		catch (const std::exception &e)
		{
			std::cout << "Unexpected exception: " << e.what() << std::endl;
		}
		++i;
	}
}

void	test_message(void)
{
	const std::string	validMessages[] = {
		"PASS password123",
		"NICK SomeNick",
		"USER username * * :Real Name",
		"JOIN #channel",
		"JOIN #channel1,#channel2,#channel3,#channel4 pass1,,pass3,*",
		"PART #channel :Goodbye!",
		"TOPIC #channel :New topic",
		"MODE #channel +it",
		"KICK #channel user :Reason for kick",
		"INVITE user #channel",
		"PRIVMSG #channel :Hello everyone!",
		"       PRIVMSG    #channel :Hello     everyone!", // extra spaces
		"JOIN #channel\t:Hello\tworld!", // tabs in the input
		":server NOTICE target :Server maintenance scheduled"
	};

	const std::string invalidMessages[] = {
		"", // Empty input
		"NICK ", // Missing nickname
		"USER username", // Incomplete USER command
		"JOIN channel", // Missing # prefix for channel
		"PART", // Missing channel name
		"TOPIC", // Missing channel and topic
		"MODE #channel +unknown_mode", // Invalid mode
		"MODE #channel +itZ", // Invalid mode after valid mode
		"KICK #channel", // Missing user and reason
		"INVITE user", // Missing channel
		"PRIVMSG", // Missing recipient and trailing
		"PRIVMSG #channel", // Missing trailing
		":server NOTICE" // Missing target and message
	};
	
	size_t	validCount = sizeof(validMessages) / sizeof(validMessages[0]);
	size_t	invalidCount = sizeof(invalidMessages) / sizeof(invalidMessages[0]);

	runTests(validMessages, validCount, "Valid", true);
	runTests(invalidMessages, invalidCount, "Invalid", false);
}

// 

void	test_arg_message(int argc, char *argv[])
{
	if (argc != 2)
		return ;

	Message msg(argv[1]);
	std::cout << msg << std::endl;
}
