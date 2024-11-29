#include "ChannelTest.hpp"

void ChannelTest::setModeTest()
{
	std::cout << "---------setMode Test---------" << std::endl;
	User naddy("Naddy", "nad");
	Channel naruto("#Naruto", naddy);
	naruto.printMembers();
	User anboisve("anboisve", "antoine");
	User gcrepin("gcrepin", "gabriel");
	User jdemers("jdemers", "JL");
	naruto.addMember(anboisve);
	naruto.addMember(jdemers);
	naruto.printMembers();
	naruto.setTopic("sasukeeeee", anboisve);
	naruto.setMode("+t", anboisve, "", NULL);
	naruto.setMode("+t", naddy, "", NULL);
	naruto.setTopic("narutooooo", anboisve);
	naruto.setTopic("narutooooo", naddy);
	naruto.setMode("+tk", naddy, "psw", NULL);
	naruto.addMember(gcrepin);
	naruto.addMember(gcrepin, "blaaaaa");
	naruto.addMember(gcrepin, "psw");
	naruto.printMembers();
	naruto.removeMember(gcrepin);
	naruto.printMembers();
	naruto.setMode("+il", naddy, "3", NULL);
	naruto.invite(gcrepin, naddy);
	naruto.setMode("+l", naddy, "4", NULL);
	naruto.invite(gcrepin, naddy);
	naruto.printMembers();
	naruto.kick(jdemers, naddy, "was bad with Gabi");
	naruto.printMembers();
	naruto.setMode("-ik", anboisve, "", NULL);
	naruto.setMode("-ik", naddy, "", NULL);
	naruto.setMode("+l", naddy, "3", NULL);
	naruto.addMember(jdemers);
	naruto.setMode("+o", naddy, "", &gcrepin);
	naruto.printMembers();
	naruto.setTopic("sasukeeee", gcrepin);
}

void ChannelTest::addRemoveTests()
{
	std::cout << "---------addRemove Tests---------" << std::endl;
	// std::vector<User> users;
	// users.push_back(User("Naddy", "Nad"));
	// Channel naruto("#Naruto", users.at(0));
	User naddy("Naddy", "nad");
	Channel naruto("#Naruto", naddy);
	naruto.printMembers();
	User anboisve("anboisve", "antoine");
	naruto.addMember(anboisve);
	naruto.printMembers();
	naruto.removeMember(anboisve);
}

void ChannelTest::commandsOpTest()
{
	std::cout << "---------commandsOp Tests---------" << std::endl;
	User naddy("Naddy", "nad");
	Channel naruto("#Naruto", naddy);
	User anboisve("anboisve", "antoine");
	User gcrepin("gcrepin", "gabriel");
	User jdemers("jdemers", "JL");
	naruto.addMember(anboisve);
	naruto.addMember(jdemers);
	naruto.setTopic("sasukeeeee", jdemers);
	naruto.setTopic("sasukeeee", naddy);
	naruto.kick(jdemers, anboisve, "You are a bad human");
	naruto.kick(jdemers, naddy, "You are a bad human");
	naruto.invite(gcrepin, anboisve);
	naruto.invite(gcrepin, naddy);
}

void ChannelTest::channelTests()
{
	addRemoveTests();
	setModeTest();
	// commandsOpTest();
}
