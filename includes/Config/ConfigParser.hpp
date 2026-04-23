/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:20:40 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/04/23 21:34:15 by maemran          ###   ########.fr       */
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