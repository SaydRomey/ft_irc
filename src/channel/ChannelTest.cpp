#include "Channel.hpp"

void setModeTest()
{
	std::cout << "---------setMode Test---------" << std::endl;
}

void addRemoveTests()
{
	std::cout << "---------addRemove Tests---------" << std::endl;
	std::vector<User> users;
	users.push_back(User("Naddy", "Nad"));
	Channel naruto("Naruto", users.at(0));
	User anboisve("anboisve", "antoine");
	naruto.addMember(anboisve);
}

void commandsOpTest()
{
	std::cout << "---------commandsOp Tests---------" << std::endl;
}

void runTests()
{
	addRemoveTests();
	setModeTest();
	commandsOpTest();
}
