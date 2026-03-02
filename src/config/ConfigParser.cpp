/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:52:45 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/02/23 11:52:46 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/ConfigParser.hpp"
#include <stdexcept>
#include <sstream>
#include <cstdlib>

/* ============================= */
/*        Constructor            */
/* ============================= */

ConfigParser::ConfigParser(const std::vector<Token>& tokens)
    : _tokens(tokens), _pos(0)
{}

/* ============================= */
/*        Core Helpers           */
/* ============================= */

bool ConfigParser::isAtEnd() const
{
    return _pos >= _tokens.size() || _tokens[_pos].type == TOKEN_EOF;
}

const Token& ConfigParser::peek() const
{
    if (_pos >= _tokens.size())
        throw std::runtime_error("Parser out of bounds");
    return _tokens[_pos];
}

const Token& ConfigParser::previous() const
{
    if (_pos == 0)
        throw std::runtime_error("Parser internal error");
    return _tokens[_pos - 1];
}

const Token& ConfigParser::advance()
{
    if (!isAtEnd())
        _pos++;
    return previous();
}

bool ConfigParser::check(TokenType type) const
{
    if (isAtEnd())
        return false;
    return peek().type == type;
}

bool ConfigParser::match(TokenType type)
{
    if (check(type))
    {
        advance();
        return true;
    }
    return false;
}

std::string ConfigParser::expectWord(const std::string& msg)
{
    if (!check(TOKEN_WORD))
        throw std::runtime_error(msg);
    return advance().value;
}

void ConfigParser::expect(TokenType type, const std::string& msg)
{
    if (!check(type))
        throw std::runtime_error(msg);
    advance();
}

/* ============================= */
/*          Entry Point          */
/* ============================= */

HttpConfig ConfigParser::parse()
{
    HttpConfig http;

    if (expectWord("Expected 'http'") != "http")
        throw std::runtime_error("Config must start with http");

    expect(TOKEN_LBRACE, "Expected '{' after http");

    while (!check(TOKEN_RBRACE) && !isAtEnd())
    {
        std::string word = expectWord("Expected directive in http");

        if (word == "server")
            http.addServer(parseServer());

        else if (word == "client_max_body_size")
        {
            http.setMaxBodySize(std::atoi(expectWord("Expected size").c_str()));
            expect(TOKEN_SEMICOLON, "Missing ';'");
        }
        else
            throw std::runtime_error("Unknown directive in http: " + word);
    }

    expect(TOKEN_RBRACE, "Missing '}' after http");
    return http;
}

/* ============================= */
/*         Server Block          */
/* ============================= */

ServerConfig ConfigParser::parseServer()
{
    ServerConfig server;

    expect(TOKEN_LBRACE, "Expected '{' after server");

    while (!check(TOKEN_RBRACE) && !isAtEnd())
        parseServerDirective(server);

    expect(TOKEN_RBRACE, "Missing '}' after server");
    return server;
}

void ConfigParser::parseServerDirective(ServerConfig& server)
{
    std::string dir = expectWord("Expected directive in server");

    if (dir == "listen")
    {
        std::string val = expectWord("Expected port");

        size_t colon = val.find(':');
        if (colon != std::string::npos)
        {
            server.setListenIp(val.substr(0, colon));
            server.setListenPort(std::atoi(val.substr(colon + 1).c_str()));
        }
        else
            server.setListenPort(std::atoi(val.c_str()));

        expect(TOKEN_SEMICOLON, "Missing ';'");
    }

    else if (dir == "root")
    {
        server.setRoot(expectWord("Expected root path"));
        expect(TOKEN_SEMICOLON, "Missing ';'");
    }

    else if (dir == "index")
    {
        server.setIndex(expectWord("Expected index"));
        expect(TOKEN_SEMICOLON, "Missing ';'");
    }

    else if (dir == "autoindex")
    {
        server.setAutoindex(expectWord("Expected on/off") == "on");
        expect(TOKEN_SEMICOLON, "Missing ';'");
    }

    else if (dir == "error_page")
    {
        int code = std::atoi(expectWord("Expected code").c_str());
        std::string path = expectWord("Expected path");
        server.addErrorPage(code, path);
        expect(TOKEN_SEMICOLON, "Missing ';'");
    }

    else if (dir == "location")
        server.addLocation(parseLocation());

    else
        throw std::runtime_error("Unknown server directive: " + dir);
}

/* ============================= */
/*       Location Block          */
/* ============================= */

LocationConfig ConfigParser::parseLocation()
{
    LocationConfig loc;

    loc.setPath(expectWord("Expected location path"));
    expect(TOKEN_LBRACE, "Missing '{' after location");

    while (!check(TOKEN_RBRACE) && !isAtEnd())
        parseLocationDirective(loc);

    expect(TOKEN_RBRACE, "Missing '}' after location");
    return loc;
}

void ConfigParser::parseLocationDirective(LocationConfig& loc)
{
    std::string dir = expectWord("Expected directive in location");

    if (dir == "root")
    {
        loc.setRoot(expectWord("Expected path"));
        expect(TOKEN_SEMICOLON, "Missing ';'");
    }

    else if (dir == "index")
    {
        loc.setIndex(expectWord("Expected index"));
        expect(TOKEN_SEMICOLON, "Missing ';'");
    }

    else if (dir == "autoindex")
    {
        loc.setAutoindex(expectWord("Expected on/off") == "on");
        expect(TOKEN_SEMICOLON, "Missing ';'");
    }

    else if (dir == "allowed_methods")
    {
        while (check(TOKEN_WORD))
            loc.addAllowedMethod(advance().value);
        expect(TOKEN_SEMICOLON, "Missing ';'");
    }

    else if (dir == "return")
    {
        int code = std::atoi(expectWord("Expected code").c_str());
        std::string url = expectWord("Expected url");
        loc.setRedirect(code, url);
        expect(TOKEN_SEMICOLON, "Missing ';'");
    }

    // else if (dir == "upload_path")
    // {
    //     loc.setUploadPath(expectWord("Expected path"));
    //     expect(TOKEN_SEMICOLON, "Missing ';'");
    // }

    else
        throw std::runtime_error("Unknown location directive: " + dir);
}