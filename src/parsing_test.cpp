/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_test.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 20:03:13 by cdumais           #+#    #+#             */
/*   Updated: 2024/11/14 20:14:51 by cdumais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
2.3.1 Message format in Augmented BNF

   The protocol messages must be extracted from the contiguous stream of
   octets.  The current solution is to designate two characters, CR and
   LF, as message separators.  Empty messages are silently ignored,
   which permits use of the sequence CR-LF between messages without
   extra problems.

   The extracted message is parsed into the components <prefix>,
   <command> and list of parameters (<params>).

    The Augmented BNF representation for this is:

    message    =  [ ":" prefix SPACE ] command [ params ] crlf
    prefix     =  servername / ( nickname [ [ "!" user ] "@" host ] )
    command    =  1*letter / 3digit
    params     =  *14( SPACE middle ) [ SPACE ":" trailing ]
               =/ 14( SPACE middle ) [ SPACE [ ":" ] trailing ]

    nospcrlfcl =  %x01-09 / %x0B-0C / %x0E-1F / %x21-39 / %x3B-FF
                    ; any octet except NUL, CR, LF, " " and ":"
    middle     =  nospcrlfcl *( ":" / nospcrlfcl )
    trailing   =  *( ":" / " " / nospcrlfcl )

    SPACE      =  %x20        ; space character
    crlf       =  %x0D %x0A   ; "carriage return" "linefeed"
*/
