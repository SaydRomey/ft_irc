/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _test_header.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:19:44 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/15 21:00:18 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_HEADER_HPP
# define TEST_HEADER_HPP

# include "Parser.hpp"
// # include "ErrorManager.hpp"

# define RESET		"\033[0m"
# define BOLD		"\033[1m"
# define ITALIC		"\033[3m"
# define UNDERLINE	"\033[4m"
# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define YELLOW		"\033[33m"
# define BLUE		"\033[34m"
# define PURPLE		"\033[35m"
# define CYAN		"\033[36m"
# define ORANGE		"\033[38;5;208m"
# define GRAYTALIC	"\033[3;90m"

int test_server(void);
int	test_parser(void);

// int	test_log_error(void);
// int	test_send_error_response(void);
// int	test_custom_err(void);

#endif // TEST_HEADER_HPP
