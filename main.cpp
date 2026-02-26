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
#include "include/config/ConfigLexer.hpp"
#include "include/config/ConfigParser.hpp"
#include "include/config/HttpConfig.hpp"
#include "include/config/LocationConfig.hpp"
#include "include/config/ServerConfig.hpp"

int main()
{
    try
    {
        // 1️⃣ Read file
        std::ifstream file("config.conf");
        if (!file.is_open())
        {
            std::cerr << "Could not open config file\n";
            return 1;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();

        // 2️⃣ Tokenize
        ConfigLexer lexer(content);
        std::vector<Token> tokens = lexer.tokenize();

        // 3️⃣ Parse
        ConfigParser parser(tokens);
        HttpConfig http = parser.parse();

        // 4️⃣ Print result
        std::cout << "Parsed successfully!\n";
        std::cout << "Number of servers: "
                  << http.getServers().size() << "\n";

        const std::vector<ServerConfig>& servers = http.getServers();

        for (size_t i = 0; i < servers.size(); i++)
        {
            std::cout << "\n--- Server " << i + 1 << " ---\n";
            std::cout << "Port: " << servers[i].getListenPort() << "\n";
            std::cout << "Root: " << servers[i].getRoot() << "\n";

            const std::vector<LocationConfig>& locations =
                servers[i].getLocations();

            for (size_t j = 0; j < locations.size(); j++)
            {
                std::cout << "  Location: "
                          << locations[j].getPath() << "\n";
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}