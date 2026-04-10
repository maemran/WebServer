/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigLexer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:52:13 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/04/10 11:35:37 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigLexer.hpp"
#include <cctype>  

ConfigLexer::ConfigLexer(const std::string& input)
    : _input(input), _pos(0), _line(1) {}

bool ConfigLexer::isSpace(char c) const
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

bool ConfigLexer::isSpecial(char c) const
{
    return (c == '{' || c == '}' || c == ';');
}

void ConfigLexer::skipSpace()
{
    while (_pos < _input.size() && isSpace(_input[_pos]))
    {
        if (_input[_pos] == '\n')
            _line++;
        _pos++;
    }
}

std::string ConfigLexer::readWord()
{
    std::string word;

    while (_pos < _input.size() &&
           !isSpace(_input[_pos]) &&
           !isSpecial(_input[_pos]))
    {
        word += _input[_pos];
        _pos++;
    }
    return word;
}

std::vector<Token> ConfigLexer::tokenize()
{
    std::vector<Token> tokens;

    while (_pos < _input.size())
    {
        skipSpace();

        if (_pos >= _input.size())
            break;

        char c = _input[_pos];

        if (_input[_pos] == '#')
        {
            while (_pos < _input.size() && _input[_pos] != '\n')
                _pos++;

            if (_pos < _input.size() && _input[_pos] == '\n')
            {
                _pos++;
                _line++;
            }

            continue;
        }
        if (c == '{')
        {
            tokens.push_back(Token(TOKEN_LBRACE, "{", _line));
            _pos++;
        }
        else if (c == '}')
        {
            tokens.push_back(Token(TOKEN_RBRACE, "}", _line));
            _pos++;
        }
        else if (c == ';')
        {
            tokens.push_back(Token(TOKEN_SEMICOLON, ";", _line));
            _pos++;
        }
        else
        {
           std::string word = readWord();
            if (!word.empty())
                tokens.push_back(Token(TOKEN_WORD, word, _line));
            else
                _pos++;
        }
    }

    tokens.push_back(Token(TOKEN_EOF, "", _line));
    return tokens;
}