#pragma once

#include <iostream>
#include "Channel.hpp"
#include "User.hpp"

class ChannelTest
{
	private:
		void setModeTest();
		void addRemovetests();
		void commandsOpTest();
	public:
		void runTests() {
			setModeTest();
			addRemovetests();
			commandsOpTest();
		}
};
