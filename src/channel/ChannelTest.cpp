#include "ChannelTest.hpp"

void ChannelTest::setModeTest()
{
	std::cout << "---------setMode Test---------" << std::endl;

	std::string topic1 = "narutoooo";
	std::string topic2 = "sasukeeeee";

	User naddy("Naddy", "nad");
	Channel naruto("#Naruto", naddy);
	naruto.printMembers();
	User anboisve("anboisve", "antoine");
	User gcrepin("gcrepin", "gabriel");
	User jdemers("jdemers", "JL");
	naruto.addMember(anboisve);
	naruto.addMember(jdemers);
	naruto.printMembers();
	naruto.setTopic(anboisve);
	naruto.setTopic(anboisve, &topic1);
	naruto.setMode("+t", anboisve, "", "", NULL);
	naruto.printMode();
	naruto.setTopic(anboisve, &topic2);
	naruto.setMode("+t", naddy, "", "", NULL);
	naruto.printMode();
	naruto.setTopic(anboisve, &topic1);
	naruto.setTopic(naddy, &topic1);
	naruto.setMode("+tk", naddy, "psw", "", NULL);
	naruto.printMode();
	naruto.addMember(gcrepin);
	naruto.addMember(gcrepin, "blaaaaa");
	naruto.addMember(gcrepin, "psw");
	naruto.printMembers();
	naruto.removeMember(gcrepin);
	naruto.printMembers();
	naruto.setMode("+il", naddy, "", "3", NULL);
	naruto.printMode();
	naruto.invite(gcrepin, naddy);
	naruto.setMode("+l", naddy, "", "4", NULL);
	naruto.printMode();
	naruto.invite(gcrepin, naddy);
	naruto.printMembers();
	naruto.kick(jdemers, naddy, "was bad with Gabi");
	naruto.printMembers();
	naruto.setMode("-ik", anboisve, "", "", NULL);
	naruto.printMode();
	naruto.setMode("-ik", naddy, "", "", NULL);
	naruto.printMode();
	naruto.setMode("+l", naddy, "", "3", NULL);
	naruto.printMode();
	naruto.addMember(jdemers);
	naruto.setMode("+o-l", naddy, "", "", &gcrepin);
	naruto.printMode();
	naruto.printMembers();
	naruto.setTopic(gcrepin, &topic1);
	naruto.setMode("+m-l", naddy, "", "", &gcrepin);
	naruto.setMode("o-l", naddy, "", "", &gcrepin);
}

void ChannelTest::addRemoveTests()
{
	std::cout << "---------addRemove Tests---------" << std::endl;
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
	
	std::string topic1 = "narutoooo";
	std::string topic2 = "sasukeeeee";
	
	User naddy("Naddy", "nad");
	Channel naruto("#Naruto", naddy);
	User anboisve("anboisve", "antoine");
	User gcrepin("gcrepin", "gabriel");
	User jdemers("jdemers", "JL");
	naruto.addMember(anboisve);
	naruto.addMember(jdemers);
	naruto.setTopic(jdemers, &topic1);
	naruto.setTopic(naddy, &topic2);
	naruto.kick(jdemers, anboisve, "You are a bad human");
	naruto.kick(jdemers, naddy, "You are a bad human");
	naruto.invite(gcrepin, anboisve);
	naruto.invite(gcrepin, naddy);
}

void ChannelTest::channelTests()
{
	addRemoveTests();
	setModeTest();
	commandsOpTest();
}
