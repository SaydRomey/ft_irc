
#include "Reply.hpp"
#include "_test_header.hpp"
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

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
** Will format the string as "Testing <testType> Replies"
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

	formattedText += " Replies" + std::string(RESET);

	return (formattedText);
}

void	printRpl(const std::string &reply, const std::string &numeric="***", const std::string &description="")
{
	std::cout << GRAYTALIC << "[" << numeric << "] " << description << RESET << std::endl;
	std::cout << "  " << reply << std::endl;
}

// static void	printSectionHeader(const std::string &title, const std::string &subtitle = "")
// {
// 	std::cout << UNDERLINE << "** " << title << " **" << RESET;
// 	if (!subtitle.empty())
// 		std::cout << GRAYTALIC << " " << subtitle << RESET;
// 	std::cout << "\n" << std::endl;
// }

static void	printInput(const std::string &input, bool prefixNewLine = false)
{
	std::cout << GRAYTALIC \
	<< (prefixNewLine ? "\n" : "") \
	<< "\"" << input << "\"" << RESET << std::endl;
}

void	test_reply(void)
{
	Reply&	rpl = Reply::getInstance();

	std::string	nickname = "HomeBoy";
	std::string	nickInUse = "SomeDude";

	printBox(formatBoxText("public member functions", PURPLE), '*', YELLOW);
	try
	{
		printRpl(rpl.reply(RPL_WELCOME, nickname, nickname), "RPL_WELCOME", "RPL_WELCOME (ReplyType enum)");
		printRpl(rpl.reply(static_cast<ReplyType>(001), nickname, nickname), "RPL_WELCOME", "static_cast<ReplyType>(001) (static_cast)");
		printRpl(rpl.reply(001, nickname, nickname), "RPL_WELCOME", "001 (int -> overload)");

		printRpl(rpl.reply(433, nickname, nickInUse), "ERR_NICKNAMEINUSE");
		printRpl(rpl.reply(461, nickname, "PRIVMSG"), "ERR_NEEDMOREPARAMS");
		printRpl(rpl.reply(462, nickname), "ERR_ALREADYREGISTERED");
	}
	catch (const std::exception& e)
	{
		std::cout << "\nError: " << e.what() << std::endl;
	}

	printBox(formatBoxText("non-member wrapper functions", PURPLE), '*', YELLOW);
	try
	{
		printRpl(reply(RPL_WELCOME, nickname, nickname), "RPL_WELCOME", "(ReplyType)");
		printRpl(reply(static_cast<ReplyType>(001), nickname, nickname), "RPL_WELCOME", "(static_cast)");
		printRpl(reply(001, nickname, nickname), "RPL_WELCOME", "(int -> overload)");

		printRpl(reply(433, nickname, nickInUse), "ERR_NICKNAMEINUSE");
		printRpl(reply(461, nickname, "PRIVMSG"), "ERR_NEEDMOREPARAMS");
		printRpl(reply(462, nickname), "ERR_ALREADYREGISTERED");
	}
	catch (const std::exception& e)
	{
		std::cout << "\nError: " << e.what() << std::endl;
	}
}

/*
Testing edge cases with Reply formatting
*/
void	test_reply_templates(void)
{
	Reply&	rpl = Reply::getInstance();

	std::string	nickname = "HomeBoy";
	std::string	nickInUse = "SomeDude";
	std::string	target = "HomeFriend";
	std::string	channel = "#channel";
	
	printBox(formatBoxText("param count for templates when formatting", PURPLE), '*', YELLOW);
	
	try
	{
		printRpl(rpl.reply(ERR_NICKNAMEINUSE, nickname, nickInUse), "ERR_NICKNAMEINUSE");
		printRpl(rpl.reply(ERR_NICKNAMEINUSE, nickname, nickInUse, "extra", "extra2"), "ERR_NICKNAMEINUSE", "(too many params)");	
	}
	catch (const std::exception& e)
	{
		std::cout << "\nError: " << e.what() << std::endl;
	}
	
	try
	{
		printRpl(rpl.reply(ERR_USERNOTINCHANNEL, nickname, target, channel), "ERR_USERNOTINCHANNEL");
		printRpl(rpl.reply(ERR_USERNOTINCHANNEL, nickname), "ERR_USERNOTINCHANNEL", "(too few params)");
	}
	catch (const std::exception& e)
	{
		std::cout << "\nError: " << e.what() << std::endl;
	}
}

void	test_pseudo_replies(void)
{
	std::string	nickname = "HonorableGuest";
	std::string	channel = "#general";
	std::string	partingMsg = "GoodBye!";
	std::string	kicker = "UptightTim";
	std::string	target = "SleepyHead";
	std::string	reason = "Too sleepy...";
	std::string	sender = "HonorableHost";

	printBox(formatBoxText("Pseudo", PURPLE), '*', YELLOW);

	printInput(":<clientNickname> JOIN :<channelName>", true);
	std::cout << joinMsg(nickname, channel) << std::endl;

	printInput(":<clientNickname> PART <channelName> [:<partingMessage>]", true);
	std::cout << partMsg(nickname, channel) << std::endl;
	std::cout << partMsg(nickname, channel, partingMsg) << std::endl;

	printInput(":<kickerNickname> KICK <channelName> <targetNickname> [:<reason>]", true);
	std::cout << kickMsg(kicker, channel, target) << std::endl;
	std::cout << kickMsg(kicker, channel, target, reason) << std::endl;
	
	printInput(":<senderNickname> INVITE <targetNickname> :<channelName>", true);
	std::cout << inviteMsg(sender, nickname, channel) << std::endl;

}

std::string getCurrentDate(void)
{
	char		buffer[80];
	std::time_t	now = std::time(0);
	std::tm		*localTime = std::localtime(&now);
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
	
	return (std::string(buffer));
}

void	test_welcome_replies(void)
{
	printBox(formatBoxText("Welcome", PURPLE), '*', YELLOW);

	// printSectionHeader("Testing Welcome Replies", "(On successful authentication)");
	
	std::string	nickname = "WelcomedGuest";
	std::string	creationDate = getCurrentDate();

	try
	{
		std::vector<std::string>	replies = generateWelcomeReplies(nickname, creationDate);
		
		if (replies.size() != 4)
		{
			std::cout << RED << "Error: Expected 4 replies, got " << replies.size() << RESET << std::endl;
			return ;
		}
		
		// printVec(replies, "Generated Replies:");
		
		printRpl(replies[0], "001", "RPL_WELCOME: Welcome message");
		printRpl(replies[1], "002", "RPL_YOURHOST: Server host and version");
		printRpl(replies[2], "003", "RPL_CREATED: Server creation date");
		printRpl(replies[3], "004", "RPL_MYINFO: Server details and channel modes");

		std::cout << GREEN << "All welcome replies validated successfully!\n" << RESET << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << RED << "Exception caught during test: " << e.what() << RESET << std::endl;
	}
}
