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
#include <cstdlib> // for atoi
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
    return peek().type == TOKEN_EOF;
}

const Token& ConfigParser::peek() const
{
    return _tokens[_pos];
}

const Token& ConfigParser::previous() const
{
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

void ConfigParser::expect(TokenType type, const std::string& message)
{
    if (check(type))
    {
        advance();
        return;
    }

    std::stringstream ss;
    ss << "Parse error at line " << peek().line << ": " << message;
    throw std::runtime_error(ss.str());
}

/* ============================= */
/*          Entry Point          */
/* ============================= */
HttpConfig ConfigParser::parse()
{
    HttpConfig http;

    // Expect "http"
    expect(TOKEN_WORD, "Expected 'http' at beginning of config");

    if (previous().value != "http")
        throw std::runtime_error("Config must start with 'http' block");

    expect(TOKEN_LBRACE, "Expected '{' after http");

    while (!check(TOKEN_RBRACE) && !isAtEnd())
    {
        expect(TOKEN_WORD, "Expected directive inside http block");
        std::string directive = previous().value;

        if (directive == "server")
        {
            http.addServer(parseServer());
        }
        else if (directive == "client_max_body_size")
        {
            expect(TOKEN_WORD, "Expected value after client_max_body_size");
            http.setMaxBodySize(std::atoi(previous().value.c_str()));
            expect(TOKEN_SEMICOLON, "Expected ';'");
        }
        else
        {
            throw std::runtime_error("Unknown directive inside http block");
        }
    }

    expect(TOKEN_RBRACE, "Expected closing '}' for http");

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
    {
        parseServerDirective(server);
    }

    expect(TOKEN_RBRACE, "Expected '}' after server block");

    return server;
}

/* ============================= */
/*       Location Block          */
/* ============================= */

LocationConfig ConfigParser::parseLocation()
{
    LocationConfig location;

    // Expect location path
    expect(TOKEN_WORD, "Expected location path");
    location.setPath(previous().value);

    expect(TOKEN_LBRACE, "Expected '{' after location path");

    while (!check(TOKEN_RBRACE) && !isAtEnd())
    {
        parseLocationDirective(location);
    }

    expect(TOKEN_RBRACE, "Expected '}' after location block");

    return location;
}

/* ============================= */
/*     Server Directives         */
/* ============================= */

void ConfigParser::parseServerDirective(ServerConfig& server)
{
    expect(TOKEN_WORD, "Expected directive inside server block");
    std::string directive = previous().value;

    if (directive == "listen")
{
    expect(TOKEN_WORD, "Expected address after listen");

    std::string value = previous().value;

    size_t colon = value.find(':');

    if (colon != std::string::npos)
    {
        server.setListenIp(value.substr(0, colon));
        server.setListenPort(std::atoi(value.substr(colon + 1).c_str()));
    }
    else
    {
        server.setListenPort(std::atoi(value.c_str()));
    }

    expect(TOKEN_SEMICOLON, "Expected ';'");
}
    else if (directive == "root")
    {
        expect(TOKEN_WORD, "Expected path after root");
        server.setRoot(previous().value);

        expect(TOKEN_SEMICOLON, "Expected ';' after root");
    }
    else if (directive == "index")
    {
        expect(TOKEN_WORD, "Expected file after index");
        server.setIndex(previous().value);

        expect(TOKEN_SEMICOLON, "Expected ';' after index");
    }
    else if (directive == "autoindex")
    {
        expect(TOKEN_WORD, "Expected on/off after autoindex");
        server.setAutoindex(previous().value == "on");

        expect(TOKEN_SEMICOLON, "Expected ';' after autoindex");
    }
    else if (directive == "location")
    {
        server.addLocation(parseLocation());
    }
    else
    {
        std::stringstream ss;
        ss << "Unknown directive '" << directive
           << "' in server block at line "
           << previous().line;
        throw std::runtime_error(ss.str());
    }
}

/* ============================= */
/*    Location Directives        */
/* ============================= */

void ConfigParser::parseLocationDirective(LocationConfig& location)
{
    expect(TOKEN_WORD, "Expected directive inside location block");
    std::string directive = previous().value;

    if (directive == "root")
    {
        expect(TOKEN_WORD, "Expected path after root");
        location.setRoot(previous().value);

        expect(TOKEN_SEMICOLON, "Expected ';' after root");
    }
    else if (directive == "index")
    {
        expect(TOKEN_WORD, "Expected file after index");
        location.setIndex(previous().value);

        expect(TOKEN_SEMICOLON, "Expected ';' after index");
    }
    else if (directive == "autoindex")
    {
        expect(TOKEN_WORD, "Expected on/off after autoindex");
        location.setAutoindex(previous().value == "on");

        expect(TOKEN_SEMICOLON, "Expected ';' after autoindex");
    }
    else
    {
        std::stringstream ss;
        ss << "Unknown directive '" << directive
           << "' in location block at line "
           << previous().line;
        throw std::runtime_error(ss.str());
    }
}