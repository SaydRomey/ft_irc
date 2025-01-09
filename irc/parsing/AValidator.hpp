
#ifndef AVALIDATOR_HPP
# define AVALIDATOR_HPP

typedef std::map<std::string, std::string>	t_mapStrStr;

class AValidator
{
	public:
		AValidator(void);
		virtual ~AValidator(void);

		virtual bool	validate(t_mapStrStr& command) const = 0;
	
	private:

};

// Derived Validators

class PassCommandValidator : public AValidator
{
	public:
		bool	validate(t_mapStrStr& command) const;
}




#endif // AVALIDATOR_HPP
