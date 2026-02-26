/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:20:40 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/02/23 11:54:56 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Responsibilities:

// Parse server block

// Parse location block

// Fill ServerConfig struct

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <vector>
#include "Token.hpp"
#include "HttpConfig.hpp"

class ConfigParser
{
private:
    std::vector<Token> _tokens;
    size_t _pos;

    // Core helpers
    bool isAtEnd() const;
    const Token& peek() const;
    const Token& previous() const;
    const Token& advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    void expect(TokenType type, const std::string& message);

    // Parsing blocks
    ServerConfig parseServer();
    LocationConfig parseLocation();

    // Directive parsing helpers
    void parseServerDirective(ServerConfig& server);
    void parseLocationDirective(LocationConfig& location);

public:
    ConfigParser(const std::vector<Token>& tokens);
    HttpConfig parse();
};

#endif