/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:42:10 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/02/23 14:44:29 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "include/config/ConfigLexer.hpp"
#include "include/config/ConfigParser.hpp"
#include "include/config/ConfigValidator.hpp"
#include "include/config/HttpConfig.hpp"
#include "include/config/ServerConfig.hpp"
#include "include/config/LocationConfig.hpp"

/* ========================================= */
/*              PRINT HELPERS                */
/* ========================================= */

void printLocation(const LocationConfig& loc)
{
    std::cout << "    📁 Location: " << loc.getPath() << "\n";
    std::cout << "       Root: " << loc.getRoot() << "\n";
    std::cout << "       Index: " << loc.getIndex() << "\n";
    std::cout << "       Autoindex: "
              << (loc.getAutoindex() ? "ON" : "OFF") << "\n";

    const std::vector<std::string>& methods = loc.getMethods();
    if (!methods.empty())
    {
        std::cout << "       Methods: ";
        for (size_t i = 0; i < methods.size(); i++)
            std::cout << methods[i] << " ";
        std::cout << "\n";
    }

    const std::map<int,std::string>& errors = loc.getErrorPages();
    for (std::map<int,std::string>::const_iterator it = errors.begin();
         it != errors.end(); ++it)
    {
        std::cout << "       ErrorPage " << it->first
                  << " -> " << it->second << "\n";
    }

    const std::map<int,std::string>& redirects = loc.getRedirections();
    for (std::map<int,std::string>::const_iterator it = redirects.begin();
         it != redirects.end(); ++it)
    {
        std::cout << "       Redirect " << it->first
                  << " -> " << it->second << "\n";
    }
}

void printServer(const ServerConfig& srv, int index)
{
    std::cout << "\n=========== SERVER " << index << " ===========\n";
    std::cout << "IP: " << srv.getListenIp() << "\n";
    std::cout << "Port: " << srv.getListenPort() << "\n";
    std::cout << "Root: " << srv.getRoot() << "\n";
    std::cout << "Index: " << srv.getIndex() << "\n";
    std::cout << "Autoindex: "
              << (srv.getAutoindex() ? "ON" : "OFF") << "\n";

    const std::vector<LocationConfig>& locs = srv.getLocations();
    for (size_t i = 0; i < locs.size(); i++)
        printLocation(locs[i]);
}

/* ========================================= */
/*                  MAIN                     */
/* ========================================= */

int main(int argc, char** argv)
{
    std::string filename = "config.conf";
    if (argc == 2)
        filename = argv[1];

    try
    {
        std::cout << "📂 Reading config: " << filename << "\n";

        /* Read file */
        std::ifstream file(filename.c_str());
        if (!file)
            throw std::runtime_error("Cannot open config file");

        std::stringstream ss;
        ss << file.rdbuf();
        std::string content = ss.str();

        /* Lexer */
        std::cout << "🔍 Tokenizing...\n";
        ConfigLexer lexer(content);
        std::vector<Token> tokens = lexer.tokenize();
        std::cout << "   Tokens count: " << tokens.size() << "\n";

        /* Parser */
        std::cout << "🧠 Parsing...\n";
        ConfigParser parser(tokens);
        HttpConfig http = parser.parse();

        std::cout << "   Servers parsed: "
                  << http.getServers().size() << "\n";
                  
        std::cout << "🎉 Config OK!\n";

        /* Print result */
        std::vector<ServerConfig>& servers = http.getServers();
        for (size_t i = 0; i < servers.size(); i++)
        {
            servers[i].resolveInheritance();
            printServer(servers[i], i + 1);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "\n❌ ERROR: " << e.what() << "\n";
        return 1;
    }
}
