
#ifndef PARSER_HPP
# define PARSER_HPP

# include <map>
# include <string>
# include <vector>

typedef std::vector<std::pair<std::string, std::string> >	t_vecPairStrStr;
typedef std::map<std::string, std::string>					t_mapStrStr;
typedef std::vector<std::string>							t_vecStr;

class Parser
{
	public:
		Parser(void);
		~Parser(void);
		
		t_mapStrStr		parseCommand(const std::string &input) const;
		t_vecPairStrStr	parseChannelsAndKeys(const std::string &params) const;
		t_vecStr		parseModeParams(const std::string &params) const;
	
	private:
		Parser(const Parser&);
		Parser& operator=(const Parser&);
};

#endif // PARSER_HPP
