
#include "CommandValidator.hpp"

CommandValidator::CommandValidator(void) {}

CommandValidator::~CommandValidator(void) {}

CommandValidator::CommandValidator(const CommandValidator &other)
{
	*this = other;
}

CommandValidator&	CommandValidator::operator=(const CommandValidator &other)
{
	// if (this != &other)
	// {}
	
	return (*this);
}

