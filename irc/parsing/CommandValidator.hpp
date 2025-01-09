
#ifndef COMMANDVALIDATOR_HPP
# define COMMANDVALIDATOR_HPP

# include <string>
# include "Message.hpp"
# include "User.hpp"
# include "Reply.hpp"

class CommandValidator
{
	public:
		CommandValidator(void);
		CommandValidator(const CommandValidator &other);
		CommandValidator&	operator=(const CommandValidator &other);
		virtual ~CommandValidator(void);

		virtual bool	validate(const Message& msg, const User& user)
	
	private:

};

#endif // COMMANDVALIDATOR_HPP
