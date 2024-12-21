/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _test_header.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:19:44 by cdumais           #+#    #+#             */
/*   Updated: 2024/12/20 22:23:13 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_HEADER_HPP
# define TEST_HEADER_HPP

# include "parsing_utils.hpp"

void	test_message(void);

void	test_reply(void);
void	test_pseudo_replies(void);
void	test_reply_templates(void);
void	test_welcome_replies(void);

void	test_tokenizer(int argc, char *argv[]);

#endif // TEST_HEADER_HPP
