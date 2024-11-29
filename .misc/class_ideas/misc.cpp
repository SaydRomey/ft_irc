
std::vector<std::string>	splitParams(const std::string &params, char delimiter)
{
	std::vector<std::string>	result;
	std::stringstream	ss(params);
	std::string	item;

	while (std::getline(ss, item, delimiter))
	{
		result.push_back(item);
	}
	return (result);
}

std::string	joinParams(const std::vector<std::string> &params)
{
	std::string	result;
	size_t	i = 0;

	while (i < params.size())
	{
		if (i > 0)
			result += " ";
		result += params[i];
		++i;
	}
	return (result);
}
