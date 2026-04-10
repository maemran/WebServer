/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:19:17 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/04/10 11:35:33 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum TokenType
{
    TOKEN_WORD,       
    TOKEN_LBRACE,      
    TOKEN_RBRACE,      
    TOKEN_SEMICOLON,   
    TOKEN_EOF         
};

struct Token
{
    TokenType type;
    std::string value;
    int line;

    Token()
        : type(TOKEN_EOF), value(""), line(0) {}

    Token(TokenType t, const std::string& v, int l)
        : type(t), value(v), line(l) {}
};

#endif