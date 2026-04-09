/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:19:17 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/03/04 15:19:33 by saabo-sh         ###   ########.fr       */
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