/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran <maemran@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 14:42:10 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/04/09 20:19:42 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

#include "ConfigLexer.hpp"
#include "ConfigParser.hpp"
#include "ConfigValidator.hpp"
#include "HttpConfig.hpp"

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

    const std::map<int, std::string>& errors = loc.getErrorPages();
    std::cout << "       Error Pages:\n";
    if (errors.empty())
        std::cout << "         None\n";
    else
    {
        for (std::map<int, std::string>::const_iterator it = errors.begin();
            it != errors.end(); ++it)
        {
            std::cout << "         " << it->first
                      << " -> " << it->second << "\n";
        }
    }

    const std::map<int, std::string>& redirections = loc.getRedirections();
    std::cout << "       Redirections:\n";
    if (redirections.empty())
        std::cout << "         None\n";
    else
    {
        for (std::map<int, std::string>::const_iterator it = redirections.begin();
            it != redirections.end(); ++it)
        {
            std::cout << "         " << it->first
                      << " -> " << it->second << "\n";
        }
    }

    const std::vector<std::string>& methods = loc.getMethods();
    std::cout << "       Allowed Methods: ";
    if (methods.empty())
        std::cout << "None";
    else
    {
        for (size_t i = 0; i < methods.size(); i++)
            std::cout << methods[i] << " ";
    }
    std::cout << "\n";

    std::cout << "       Client Max Body Size: "
              << loc.getMaxBodySize() << " bytes\n";

    std::cout << "       Has Redirect: "
              << (loc.hasRedirect() ? "YES" : "NO") << "\n";
    std::cout << "       Redirect Code: " << loc.getRedirectCode() << "\n";
    std::cout << "       Redirect URL: " << loc.getRedirectUrl() << "\n";

    const std::map<std::string, std::string>& cgi = loc.getCgiMap();
    std::cout << "       CGI:\n";
    if (cgi.empty())
        std::cout << "         None\n";
    else
    {
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
    std::cout << "\n=========== ServerConfig " << i << " ===========\n";

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

    const std::vector<std::string>& methods = srv.getMethods();
    std::cout << "Allowed Methods: ";
    if (methods.empty())
        std::cout << "None";
    else
    {
        for (size_t j = 0; j < methods.size(); j++)
            std::cout << methods[j] << " ";
    }
    std::cout << "\n";

    const std::map<int, std::string>& errors = srv.getErrorPages();
    std::cout << "Error Pages:\n";
    if (errors.empty())
        std::cout << "  None\n";
    else
    {
        for (std::map<int, std::string>::const_iterator it = errors.begin();
            it != errors.end(); ++it)
        {
            std::cout << "  " << it->first << " -> " << it->second << "\n";
        }
    }

    std::cout << "Client Max Body Size: "
          << srv.getMaxBodySize() << " bytes\n";

    const std::vector<LocationConfig>& locs = srv.getLocations();

    std::cout << "Locations: " << locs.size() << "\n";
    if (locs.empty())
        std::cout << "  None\n";

    for (size_t j = 0; j < locs.size(); j++)
	{
		std::cout << "============================" <<std::endl;
		std::cout << "==========Lcation===========" <<std::endl;
		std::cout << "============================" <<std::endl;
        testLocation(locs[j]);
	}
}

/* ===================================== */
/*             PRINT HTTP               */
/* ===================================== */

void testHttp(const HttpConfig& http)
{
    std::cout << "\n=========== HttpConfig ===========\n";

    std::cout << "Root: " << http.getRoot();
    if (!http.getRoot().empty())
        std::cout << (pathExists(http.getRoot()) ? " ✅" : " ❌");
    std::cout << "\n";

    std::cout << "Index: ";
    printIndexFiles(http.getIndexFiles());
    std::cout << "\n";

    std::cout << "Autoindex: "
              << (http.getAutoindex() ? "ON" : "OFF") << "\n";
    std::cout << "Client Max Body Size: "
              << http.getMaxBodySize() << " bytes\n";

    const std::vector<std::string>& methods = http.getMethods();
    if (!methods.empty())
    {
        std::cout << "Methods: ";
        for (size_t i = 0; i < methods.size(); i++)
            std::cout << methods[i] << " ";
        std::cout << "\n";
    }

    const std::map<int, std::string>& errors = http.getErrorPages();
    if (!errors.empty())
    {
        std::cout << "Error Pages:\n";
        for (std::map<int, std::string>::const_iterator it = errors.begin();
            it != errors.end(); ++it)
        {
            std::cout << "  " << it->first
                      << " -> " << it->second << "\n";
        }
    }
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

        testHttp(http);

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
//#include "serversocket.hpp"
//#include "HttpConfig.hpp"
//#include "iostream"
//int main()
//{
//    try
//    {
//        HttpConfig config;
//        ServerSocket server(config);

//        server.start();
//        server.run();
//    }
//    catch(const std::exception& e)
//    {
//        std::cerr << e.what() << '\n';
//    }
//}