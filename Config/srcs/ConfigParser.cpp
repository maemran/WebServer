/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran <maemran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:52:45 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/04/09 20:19:37 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include <stdexcept>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <cstdio>

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
        {
            http.addServer(parseServer());
        }

        else if (word == "root")
        {
            http.setRoot(expectWord("Expected root path"));
            expect(TOKEN_SEMICOLON, "Missing ';'");
        }

       else if (word == "index")
        {
            if (!check(TOKEN_WORD))
                throw std::runtime_error("index directive requires at least one file");

            while (check(TOKEN_WORD))
                http.addIndexFile(advance().value);

            expect(TOKEN_SEMICOLON, "Missing ';'");
        }
        else if (word == "autoindex")
        {
            std::string val = expectWord("Expected 'on' or 'off'");

            if (val != "on" && val != "off")
                throw std::runtime_error("Invalid autoindex value: " + val);

            http.setAutoindex(val == "on");

            expect(TOKEN_SEMICOLON, "Missing ';'");
        }
        else if (word == "client_max_body_size")
        {
            std::string sizeStr = expectWord("Expected size");
            http.setMaxBodySize(parseSize(sizeStr));
            expect(TOKEN_SEMICOLON, "Missing ';'");
        }

        else if (word == "error_page")
        {
            int code = std::atoi(expectWord("Expected error code").c_str());
            std::string path = expectWord("Expected error page path");
            http.addErrorPage(code, path);
            expect(TOKEN_SEMICOLON, "Missing ';'");
        }

        else
        {
            throw std::runtime_error("Unknown directive in http: " + word);
        }
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
        std::string val = expectWord("Expected listen value");

        size_t colon = val.find(':');

        if (colon != std::string::npos)
        {
            // IP:PORT
            server.setListenIp(val.substr(0, colon));
            server.setListenPort(std::atoi(val.substr(colon + 1).c_str()));
        }
        else
        {
            // Check if it's a number (PORT)
            bool isNumber = true;
            for (size_t i = 0; i < val.length(); i++)
            {
                if (!isdigit(val[i]))
                {
                    isNumber = false;
                    break;
                }
            }

            if (isNumber)
            {
                // PORT only
                server.setListenPort(std::atoi(val.c_str()));
            }
            else
            {
                // IP only → default port
                server.setListenIp(val);
                server.setListenPort(80); // or 8080 (your choice)
            }
        }
        expect(TOKEN_SEMICOLON, "Missing ';'");
    }
    else if (dir == "root")
    {
        server.setRoot(expectWord("Expected root path"));
        expect(TOKEN_SEMICOLON, "Missing ';'");
    }

    else if (dir == "index")
    {
        while (check(TOKEN_WORD))
            server.addIndexFile(advance().value);

        expect(TOKEN_SEMICOLON, "Missing ';'");
    }

    else if (dir == "autoindex")
    {
        server.setAutoindex(expectWord("Expected on/off") == "on");
        expect(TOKEN_SEMICOLON, "Missing ';'");
    }

    else if (dir == "client_max_body_size")
    {
        std::string sizeStr = expectWord("Expected size");
        server.setMaxBodySize(parseSize(sizeStr));
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
    {
        server.addLocation(parseLocation());
    }

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
        if (!check(TOKEN_WORD))
            throw std::runtime_error("index directive requires at least one file");

        while (check(TOKEN_WORD))
            loc.addIndexFile(advance().value);

        expect(TOKEN_SEMICOLON, "Missing ';'");
    }
    
    else if (dir == "autoindex")
    {
        std::string val = expectWord("Expected 'on' or 'off'");

        if (val != "on" && val != "off")
            throw std::runtime_error("Invalid autoindex value: " + val);

    loc.setAutoindex(val == "on");

        expect(TOKEN_SEMICOLON, "Missing ';'");
}

    else if (dir == "client_max_body_size")
    {
        std::string sizeStr = expectWord("Expected size");
        loc.setMaxBodySize(parseSize(sizeStr));
        expect(TOKEN_SEMICOLON, "Missing ';'");
    }

    else if (dir == "allowed_methods")
    {
        if (!check(TOKEN_WORD))
            throw std::runtime_error("allowed methods directive requires method");

        while (check(TOKEN_WORD))
            loc.addAllowedMethod(advance().value);

        expect(TOKEN_SEMICOLON, "Missing ';'");
    }

    else if (dir == "return")
    {
        int code = std::atoi(expectWord("Expected code").c_str());
        std::string url = expectWord("Expected url");
        
    
        if (url.empty() || (url[0] != '/' && url.find("http://") != 0))
        {
            throw std::runtime_error(
            "Invalid redirect URL: must start with '/' or http:// ");
        }
       
        if (code / 100 == 3) {
            loc.setRedirect(code, url);
        }
        else
            throw std::runtime_error("Invalid redirect status code");
        
        
        
        expect(TOKEN_SEMICOLON, "Missing ';'");
        }

        else if (dir == "error_page")
        {
            int code = std::atoi(expectWord("Expected error code").c_str());
            std::string path = expectWord("Expected error page path");

            loc.addErrorPage(code, path);

            expect(TOKEN_SEMICOLON, "Missing ';'");
        }
        else if (dir == "cgi")
        {
            std::string ext = expectWord("Expected CGI extension");
            std::string path = expectWord("Expected CGI executable");

            if (ext[0] != '.')
                throw std::runtime_error("CGI extension must start with '.'");

            loc.addCgi(ext, path);

            expect(TOKEN_SEMICOLON, "Missing ';'");
        }
        
        else
            throw std::runtime_error("Unknown location directive: " + dir);
}

size_t ConfigParser::parseSize(const std::string& value)
{
    if (value.empty())
        throw std::runtime_error("Invalid client_max_body_size");

    size_t multiplier = 1;
    std::string numberPart = value;

    char last = value[value.size() - 1];

    if (last == 'K' || last == 'k')
    {
        multiplier = 1024;
        numberPart = value.substr(0, value.size() - 1);
    }
    else if (last == 'M' || last == 'm')
    {
        multiplier = 1024 * 1024;
        numberPart = value.substr(0, value.size() - 1);
    }
    else if (last == 'G' || last == 'g')
    {
        multiplier = 1024 * 1024 * 1024;
        numberPart = value.substr(0, value.size() - 1);
    }

    size_t number = std::atoi(numberPart.c_str());

    if (number == 0)
        throw std::runtime_error("Invalid size format");

    return number * multiplier;
}