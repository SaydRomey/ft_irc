/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:10:19 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/15 12:56:07 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_HPP
# define TOKENIZER_HPP

# include <string>
# include <vector>

class Tokenizer
{
	public:
		Tokenizer(void);
		std::vector<std::string>	tokenize(const std::string &input);
	
	private:
		void	_split(const std::string &str, char delimiter, std::vector<std::string> &tokens);
};

#endif // TOKENIZER_HPP
