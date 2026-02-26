/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigLexer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:19:54 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/02/23 12:25:20 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Responsibilities:

// Read config text

// Produce vector<Token>
#ifndef CONFIGLEXER_HPP
#define CONFIGLEXER_HPP

#include <string>
#include <vector>
#include "Token.hpp"

/*
** ConfigLexer
** -----------
** Reads raw config file text and converts it into tokens.
*/
class ConfigLexer
{
private:
    std::string _input;
    size_t      _pos;
    int         _line;

    bool    isSpace(char c) const;
    bool    isSpecial(char c) const;
    void    skipSpace();
    std::string readWord();

public:
    ConfigLexer(const std::string& input);

    std::vector<Token> tokenize();
};

#endif