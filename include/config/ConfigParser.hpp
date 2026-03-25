/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:20:40 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/03/24 14:51:21 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    std::string expectWord(const std::string& message);


    ServerConfig parseServer();
    LocationConfig parseLocation();

    void parseServerDirective(ServerConfig& server);
    void parseLocationDirective(LocationConfig& location);
    size_t parseSize(const std::string& value);

public:
    ConfigParser(const std::vector<Token>& tokens);
    HttpConfig parse();
};

#endif