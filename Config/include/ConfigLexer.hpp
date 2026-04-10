/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigLexer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:19:54 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/04/10 11:34:48 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGLEXER_HPP
#define CONFIGLEXER_HPP

#include <string>
#include <vector>
#include "Token.hpp"

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