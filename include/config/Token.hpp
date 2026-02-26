/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:19:17 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/02/23 12:12:56 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

/*
** TokenType
** ----------
** Represents the type of token found in the config file.
*/
enum TokenType
{
    TOKEN_WORD,       
    TOKEN_LBRACE,      
    TOKEN_RBRACE,      
    TOKEN_SEMICOLON,   
    TOKEN_EOF         
};

/*
** Token
** ------
** Represents one lexical unit from the config file.
*/
struct Token
{
    TokenType type;
    std::string value;
    int line;

    // Default constructor
    Token()
        : type(TOKEN_EOF), value(""), line(0) {}

    // Parameterized constructor
    Token(TokenType t, const std::string& v, int l)
        : type(t), value(v), line(l) {}
};

#endif