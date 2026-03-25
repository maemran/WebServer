/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:42:10 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/03/25 10:56:10 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

#include "config/ConfigLexer.hpp"
#include "config/ConfigParser.hpp"
#include "config/ConfigValidator.hpp"
#include "config/HttpConfig.hpp"

/* ===================================== */
/*         FILESYSTEM CHECK              */
/* ===================================== */

bool pathExists(const std::string& path)
{
    struct stat info;
    return (stat(path.c_str(), &info) == 0);
}

/* ===================================== */
/*           PRINT INDEX FILES           */
/* ===================================== */

void printIndexFiles(const std::vector<std::string>& indexFiles)
{
    if (indexFiles.empty())
    {
        std::cout << "None";
        return;
    }

    for (size_t i = 0; i < indexFiles.size(); i++)
        std::cout << indexFiles[i] << " ";
}

/* ===================================== */
/*           PRINT LOCATION              */
/* ===================================== */

void testLocation(const LocationConfig& loc)
{
    std::cout << "    📁 Location: " << loc.getPath() << "\n";

    std::cout << "       Root: " << loc.getRoot();

    if (!loc.getRoot().empty())
        std::cout << (pathExists(loc.getRoot()) ? " ✅" : " ❌");

    std::cout << "\n";

    std::cout << "       Index: ";
    printIndexFiles(loc.getIndexFiles());
    std::cout << "\n";

    std::cout << "       Autoindex: "
              << (loc.getAutoindex() ? "ON" : "OFF") << "\n";
    
            //   std::cout << "DEBUG: hasRedirect = " << loc.hasRedirect() << std::endl;
      const std::vector<std::string>& methods = loc.getMethods();
        const std::map<int, std::string>& errors = loc.getErrorPages();

        if (!errors.empty())
        {
            std::cout << "       Error Pages:\n";
            for (std::map<int, std::string>::const_iterator it = errors.begin();
                it != errors.end(); ++it)
            {
                std::cout << "         " << it->first
                        << " -> " << it->second << "\n";
            }
        }
    if (!methods.empty())
    {
        std::cout << "       Methods: ";
        for (size_t i = 0; i < methods.size(); i++)
            std::cout << methods[i] << " ";
        std::cout << "\n";
    }

    const std::map<std::string, std::string>& cgi = loc.getCgiMap();

        if (!cgi.empty())
        {
            std::cout << "       CGI:\n";
            for (std::map<std::string, std::string>::const_iterator it = cgi.begin();
                it != cgi.end(); ++it)
            {
                std::cout << "         " << it->first
                        << " -> " << it->second << "\n";
            }
        }
}

/* ===================================== */
/*            PRINT SERVER               */
/* ===================================== */

void testServer(ServerConfig& srv, int i)
{
    std::cout << "\n=========== SERVER " << i << " ===========\n";

    std::cout << "IP: " << srv.getListenIp() << "\n";
    std::cout << "Port: " << srv.getListenPort() << "\n";

    std::cout << "Root: " << srv.getRoot();
    if (!srv.getRoot().empty())
        std::cout << (pathExists(srv.getRoot()) ? " ✅" : " ❌");
    std::cout << "\n";

    std::cout << "Index: ";
    printIndexFiles(srv.getIndexFiles());
    std::cout << "\n";

    std::cout << "Autoindex: "
              << (srv.getAutoindex() ? "ON" : "OFF") << "\n";
    std::cout << "Client Max Body Size: "
          << srv.getMaxBodySize() << " bytes\n";

    const std::vector<LocationConfig>& locs = srv.getLocations();

    for (size_t j = 0; j < locs.size(); j++)
        testLocation(locs[j]);
}

/* ===================================== */
/*                MAIN                   */
/* ===================================== */

int main(int argc, char** argv)
{
    std::string filename = "config.conf";
    if (argc == 2)
        filename = argv[1];

    try
    {
        std::cout << "📂 Reading config: " << filename << "\n";

        /* ---------- Read file ---------- */

        std::ifstream file(filename.c_str());
        if (!file.is_open())
            throw std::runtime_error("Cannot open config file");

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();

        /* ---------- Lexer ---------- */

        std::cout << "🔍 Tokenizing...\n";
        ConfigLexer lexer(content);
        std::vector<Token> tokens = lexer.tokenize();

        std::cout << "Tokens: " << tokens.size() << "\n";

        /* ---------- Parser ---------- */

        std::cout << "🧠 Parsing...\n";
        ConfigParser parser(tokens);
        HttpConfig http = parser.parse();

        /* ---------- Validator ---------- */

            
        std::cout << "✅ Validating...\n";
        ConfigValidator validator;
        validator.validate(http);

        
        std::cout << "🎉 Config VALID\n";

        /* ---------- TEST OUTPUT ---------- */

        std::vector<ServerConfig>& servers = http.getServers();

        for (size_t i = 0; i < servers.size(); i++)
            testServer(servers[i], i + 1);
    }
    catch (std::exception& e)
    {
        std::cerr << "\n❌ ERROR: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
