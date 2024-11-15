/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:16:28 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/14 16:03:24 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTING_HPP
# define TESTING_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <iostream>
# include <cstring>

# define IRC_PORT 6667 // Default IRC port

int test_server(void);

#endif // TESTING_HPP
